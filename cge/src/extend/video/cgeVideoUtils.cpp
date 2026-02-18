/*
* cgeVideoUtils.cpp
*
*  Created on: 2015-12-10
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#include "cgeGLFunctions.h"
#include "cgeVideoUtils.h"
#include "cgeFFmpegHeaders.h"
#include "cgeVideoDecoder.h"
#include "cgeVideoEncoder.h"
#include "cgeVideoPlayer.h"
#include "cgeBlendFilter.h"
#include "cgeMultipleEffects.h"
#include "cgeTextureUtils.h"

static void get_audio_frame(int16_t *samples, int frame_size, int nb_channels)
{
	int j, i, v;
	int16_t *q;

	static float t = 0, 
		tincr = 2 * M_PI * 110.0 / 44100,
		tincr2 = 2 * M_PI * 110.0 / 44100.0f / 44100.0f;

	q = samples;
	for (j = 0; j < frame_size; j++) {
		v = (int)(sin(t) * 10000);
		for (i = 0; i < nb_channels; i++)
			*q++ = v;
		t     += tincr;
		tincr += tincr2;
	}
}

namespace CGE
{
	void CGEFastFrameHandler::processingFilters()
	{
		if(m_vecFilters.empty() || m_bufferTextures[0] == 0)
		{
			glFlush();
			return;
		}

		glDisable(GL_BLEND);
		assert(m_vertexArrayBuffer != 0);

		glViewport(0, 0, m_dstImageSize.width, m_dstImageSize.height);

		for(std::vector<CGEImageFilterInterfaceAbstract*>::iterator iter = m_vecFilters.begin();
			iter < m_vecFilters.end(); ++iter)
		{
			swapBufferFBO();
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffer);
			(*iter)->render2Texture(this, m_bufferTextures[1], m_vertexArrayBuffer);
			glFlush();
		}
		glFinish();
		// glFlush();
	}

	void CGEFastFrameHandler::swapBufferFBO()
	{
		useImageFBO();
		std::swap(m_bufferTextures[0], m_bufferTextures[1]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_bufferTextures[0], 0);

		//For performance, errors are not checked during high-framerate rendering.
	}

	bool cgeGenerateVideoWithFilter(const char* outputFilename, const char* inputFilename, const char* filterConfig, float filterIntensity, GLuint texID, CGETextureBlendMode blendMode, float blendIntensity)
	{
        static const int ENCODE_FPS = 30;

		CGEVideoDecodeHandler* decodeHandler = new CGEVideoDecodeHandler();

		if(!decodeHandler->open(inputFilename))
		{
			CGE_LOG_ERROR("Open %s failed!\n", inputFilename);
			delete decodeHandler;
			return false;
		}

//		decodeHandler->setSamplingStyle(CGEVideoDecodeHandler::ssFastBilinear); //already default

		int videoWidth = decodeHandler->getWidth();
		int videoHeight = decodeHandler->getHeight();
		unsigned char* cacheBuffer = nullptr;
		int cacheBufferSize = 0;

		CGEVideoPlayerYUV420P videoPlayer;
		videoPlayer.initWithDecodeHandler(decodeHandler);

		CGEVideoEncoderMP4 mp4Encoder;

		mp4Encoder.setRecordDataFormat(CGEVideoEncoderMP4::FMT_RGBA8888);
		if(!mp4Encoder.init(outputFilename, ENCODE_FPS, videoWidth, videoHeight, true))
		{
			CGE_LOG_ERROR("CGEVideoEncoderMP4 - start recording failed!");
			return false;
		}

		CGE_LOG_INFO("encoder created!");

		CGEFastFrameHandler handler;
		CGEBlendFilter* blendFilter = nullptr;

		if(texID != 0 && blendIntensity != 0.0f)
		{
			blendFilter = new CGEBlendFilter();

			if(blendFilter->initWithMode(blendMode))
			{
				blendFilter->setSamplerID(texID);
				blendFilter->setIntensity(blendIntensity);
			}
			else
			{
				delete blendFilter;
				blendFilter = nullptr;
			}
		}

		bool hasFilter = blendFilter != nullptr || (filterConfig != nullptr && *filterConfig != '\0' && filterIntensity != 0.0f);

		CGE_LOG_INFO("Has filter: %d\n", (int)hasFilter);

		if(hasFilter)
		{
			handler.initWithRawBufferData(nullptr, videoWidth, videoHeight, CGE_FORMAT_RGBA_INT8, false);
            handler.getResultDrawer()->setFlipScale(1.0f, -1.0f);
			if(filterConfig != nullptr && *filterConfig != '\0' && filterIntensity != 0.0f)
			{
				CGEMutipleEffectFilter* filter = new CGEMutipleEffectFilter;
				//filter->setTextureLoadFunction(loadTexFunc, &texLoadArg);
				filter->initWithEffectString(filterConfig);
				filter->setIntensity(filterIntensity);
				handler.addImageFilter(filter);
			}

			if(blendFilter != nullptr)
			{
				handler.addImageFilter(blendFilter);
			}

			cacheBufferSize = videoWidth * videoHeight * 4;
			cacheBuffer = new unsigned char[cacheBufferSize];
		}

		int videoPTS = -1;

		while(1)
		{
			CGEFrameTypeNext nextFrameType = videoPlayer.queryNextFrame();

			if(nextFrameType == FrameType_VideoFrame)
			{
				if(!videoPlayer.updateVideoFrame())
					continue;

                int newPTS = decodeHandler->getCurrentTimestamp() / 1000.0 * ENCODE_FPS;

                CGE_LOG_INFO("last pts: %d, new pts; %d\n", videoPTS, newPTS);

                if(videoPTS < 0)
                {
                    videoPTS = 0;
                }
                else if(videoPTS < newPTS)
                {
                    // Avoid frame drop
                    if(videoPTS + 3 > newPTS)
                    {
                        ++videoPTS;
                    }
                    else
                    {
                        videoPTS = newPTS;
                    }                    
                }
                else
                {
                    CGE_LOG_ERROR("drop frame...\n");
                    continue;
                }

				if(hasFilter)
				{
					handler.setAsTarget();
					videoPlayer.render();
					glViewport(0, 0, videoPlayer.getLinesize(), videoHeight);
					handler.processingFilters();

#if 0
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					glViewport(0, 0, videoWidth, videoHeight);
					handler.drawResult();
					glFinish();

					glReadPixels(0, 0, videoWidth, videoHeight, GL_RGBA, GL_UNSIGNED_BYTE, cacheBuffer);
#else

					handler.getOutputBufferData(cacheBuffer, CGE_FORMAT_RGBA_INT8);
#endif

					CGEVideoEncoderMP4::ImageData imageData;
					imageData.width = videoWidth;
					imageData.height = videoHeight;
					imageData.linesize[0] = videoWidth * 4;
					imageData.data[0] = cacheBuffer;
					imageData.pts = videoPTS;

					if(!mp4Encoder.record(imageData))
					{
						CGE_LOG_ERROR("record frame failed!");
					}
				}
				else
				{
					AVFrame* frame = decodeHandler->getCurrentVideoAVFrame();

					frame->pts = videoPTS;
					if(frame->data[0] == nullptr)
						continue;
					mp4Encoder.recordVideoFrame(frame);
				}
			}
			else if(nextFrameType == FrameType_AudioFrame)
			{
				AVFrame* pAudioFrame = decodeHandler->getCurrentAudioAVFrame();
                
				if(pAudioFrame == nullptr)
					continue;

				mp4Encoder.recordAudioFrame(pAudioFrame);
			}
			else
			{
				break;
			}
		}

		mp4Encoder.save();			
		delete[] cacheBuffer;
		
		return true;
	}
}