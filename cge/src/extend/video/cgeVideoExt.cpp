/*
* cgeVideoExt.cpp
*
*  Created on: 2014-9-18
*      Author: Wang Yang
*        Mail: admin@wysaid.org
* Description: Parts of this code are directly from the official ffmpeg demo
*/

#ifndef _CGE_EXT_NO_VIDEO_

#include "cgeVideoExt.h"
#include <fstream>
#include <cassert>
#include <algorithm>
#include "cgeGLFunctions.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"
#include "libswresample/swresample.h"
}
#ifdef _MSC_VER
#pragma comment(lib,"ffmpeg/lib/avcodec.lib")
#pragma comment(lib,"ffmpeg/lib/avdevice.lib")
#pragma comment(lib,"ffmpeg/lib/avfilter.lib")
#pragma comment(lib,"ffmpeg/lib/avformat.lib")
#pragma comment(lib,"ffmpeg/lib/avutil.lib")
#pragma comment(lib,"ffmpeg/lib/swscale.lib")
#pragma comment(lib,"ffmpeg/lib/swresample.lib")
#endif

static AVStream *addStream(AVFormatContext *oc, AVCodec **codec,
						   enum AVCodecID codec_id, int frameRate, int width = -1, int height = -1)
{
	AVCodecContext *c;
	AVStream *st;

	/* find the encoder */
	*codec = avcodec_find_encoder(codec_id);
	if (!(*codec)) {
		CGE_LOG_ERROR("Could not find encoder for '%s'\n", avcodec_get_name(codec_id));
		return nullptr;
	}

	st = avformat_new_stream(oc, *codec);
	if (!st) {
		CGE_LOG_ERROR("Could not allocate stream\n");
		return nullptr;
	}
	st->id = oc->nb_streams-1;
	c = st->codec;

	switch ((*codec)->type) {
	case AVMEDIA_TYPE_AUDIO:
		c->sample_fmt  = AV_SAMPLE_FMT_FLTP;
		c->bit_rate    = 64000;
		c->sample_rate = 44100;
		c->channels    = 1;
		c->flags      |= CODEC_FLAG_GLOBAL_HEADER;
		c->strict_std_compliance = -2;
		break;

	case AVMEDIA_TYPE_VIDEO:
		c->codec_id = codec_id;

		c->bit_rate = 800000;
		/* Resolution must be a multiple of two. */
		c->width    = width;
		c->height   = height;
		/* timebase: This is the fundamental unit of time (in seconds) in terms
		* of which frame timestamps are represented. For fixed-fps content,
		* timebase should be 1/framerate and timestamp increments should be
		* identical to 1. */
		c->time_base.den = frameRate;
		c->time_base.num = 1;
		c->gop_size      = 12; /* emit one intra frame every twelve frames at most */
		c->pix_fmt       = AV_PIX_FMT_YUV420P;

		av_opt_set(c->priv_data, "preset", "veryfast", 0);
		if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
			/* just for testing, we also add B frames */
			c->max_b_frames = 2;
		}
		if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
			/* Needed to avoid using macroblocks in which some coeffs overflow.
			* This does not happen with normal video, it just happens here as
			* the motion of the chroma plane does not match the luma plane. */
			c->mb_decision = 2;
		}
		break;

	default:
		break;
	}

	/* Some formats want stream headers to be separate. */
	if (oc->oformat->flags & AVFMT_GLOBALHEADER)
		c->flags |= CODEC_FLAG_GLOBAL_HEADER;

	return st;
}

namespace CGE
{
	bool cgeInitVideoExt()
	{
		av_register_all();
		return true;
	}

	struct CGEVideoDecodeContext
	{
		CGEVideoDecodeContext() : pFormatCtx(nullptr), pVideoCodecCtx(nullptr), pAudioCodecCtx(nullptr), pVideoCodec(nullptr), pAudioCodec(nullptr), pVideoFrame(nullptr), pVideoFrameRGB(nullptr), pAudioFrame(nullptr), pVideoStream(nullptr), pAudioStream(nullptr), videoStreamIndex(-1), audioStreamIndex(-1) {}
		~CGEVideoDecodeContext()
		{
			cleanup();
		}

		inline void cleanup()
		{
			if(pVideoCodecCtx != nullptr)
			{
				avcodec_close(pVideoCodecCtx);
				pVideoCodecCtx = nullptr;
			}
			
			if(pAudioCodecCtx)
			{
				avcodec_close(pAudioCodecCtx);
				pAudioCodecCtx = nullptr;
			}
			
			if(pFormatCtx != nullptr)
			{
				avformat_close_input(&pFormatCtx);
				pFormatCtx = nullptr;
			}
			
			av_free(pVideoFrame);
			av_free(pVideoFrameRGB);
			av_free(pAudioFrame);
			pVideoFrame = nullptr;
			pVideoFrameRGB = nullptr;
			pAudioFrame = nullptr;

			videoStreamIndex = -1;
			audioStreamIndex = -1;
		}

		AVFormatContext *pFormatCtx;
		AVCodecContext  *pVideoCodecCtx;
		AVCodecContext	*pAudioCodecCtx;

		AVCodec         *pVideoCodec;
		AVCodec			*pAudioCodec;

		AVFrame         *pVideoFrame;
		AVFrame         *pVideoFrameRGB;
		AVFrame			*pAudioFrame;

		AVPacket        packet;
		AVStream        *pVideoStream;
		AVStream		*pAudioStream;

		int				videoStreamIndex;
		int				audioStreamIndex;
	};

	CGEVideoDecodeHandler::CGEVideoDecodeHandler() : m_bufferPtr(nullptr), m_width(0), m_height(0), m_samplingStyle(ssFastBilinear), m_currentTimestamp(0.0)
	{
		m_context = new CGEVideoDecodeContext();
		memset(&m_cachedVideoFrame, 0, sizeof(m_cachedVideoFrame));
		memset(&m_cachedAudioFrame, 0, sizeof(m_cachedAudioFrame));
	}

	CGEVideoDecodeHandler::~CGEVideoDecodeHandler()
	{
		close();
	}

	bool CGEVideoDecodeHandler::open(const char* filename)
	{		
		CGEVideoDecodeContext& context = *(CGEVideoDecodeContext*)m_context;

		if(avformat_open_input(&context.pFormatCtx, filename, nullptr, nullptr)!=0 ||
			avformat_find_stream_info(context.pFormatCtx, nullptr)<0)
		{
			return false;  //Decoding failed
		}

		av_dump_format(context.pFormatCtx, 0, filename, 0);
		context.videoStreamIndex = -1;
		context.audioStreamIndex = -1;

		for(unsigned int i = 0; i < context.pFormatCtx->nb_streams; i++)  
		{
			if(context.pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
			{  
				context.videoStreamIndex = i;
				context.pVideoStream = context.pFormatCtx->streams[i];
				context.pVideoCodecCtx = context.pFormatCtx->streams[i]->codec;
			}
			else if(context.pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
			{
				context.audioStreamIndex = i;
				context.pAudioStream = context.pFormatCtx->streams[i];
				context.pAudioCodecCtx = context.pFormatCtx->streams[i]->codec;
			}
		}

		if(context.videoStreamIndex == -1)
		{
			return false; //Could not find video stream
		}

		if(context.audioStreamIndex == -1)
		{
			CGE_LOG_INFO("No audio stream found, video will be silent...\n");
		}

		//////////////////////////////////////////////////////////////////////////

		context.pVideoCodec = avcodec_find_decoder(context.pVideoCodecCtx->codec_id);

		if(context.pVideoCodec == nullptr || avcodec_open2(context.pVideoCodecCtx, context.pVideoCodec, nullptr) < 0)
		{
			return false; //Video decoding failed
		}

		if(context.audioStreamIndex != -1)
		{
			context.pAudioCodec = avcodec_find_decoder(context.pAudioCodecCtx->codec_id);

			if(context.pAudioCodec == nullptr || avcodec_open2(context.pAudioCodecCtx, context.pAudioCodec, nullptr) < 0)
			{
				CGE_LOG_ERROR("Audio decoding failed! Falling back to silence...\n");
				context.audioStreamIndex = -1;
				context.pAudioCodec = nullptr;
				context.pAudioCodecCtx = nullptr;
			}
		}

		m_width = context.pVideoCodecCtx->width;
		m_height = context.pVideoCodecCtx->height;

		context.pVideoFrame = av_frame_alloc();
		context.pAudioFrame = av_frame_alloc();

		av_init_packet(&context.packet);
		context.packet.data = nullptr;
		context.packet.size = 0;

		return context.pVideoFrame != nullptr && context.pAudioFrame != nullptr;// && initFrameRGB();
	}

	bool CGEVideoDecodeHandler::initFrameRGB()
	{
		CGEVideoDecodeContext& context = *(CGEVideoDecodeContext*)m_context;

		if(context.pVideoFrameRGB == nullptr)
			context.pVideoFrameRGB = av_frame_alloc();  

		if(context.pVideoFrame == nullptr || context.pVideoFrameRGB == nullptr)
		{
			return false; //Failed to allocate frame.
		}

		int numBytes = avpicture_get_size(PIX_FMT_RGBA, context.pVideoCodecCtx->width,
			context.pVideoCodecCtx->height);  
		m_bufferPtr = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t)); 

		avpicture_fill((AVPicture *)context.pVideoFrameRGB, m_bufferPtr, PIX_FMT_RGBA, context.pVideoCodecCtx->width, context.pVideoCodecCtx->height);  
		return true;
	}

	void CGEVideoDecodeHandler::close()
	{
		if(m_context == nullptr)
			return;
		CGEVideoDecodeContext& context = *(CGEVideoDecodeContext*)m_context;		
		av_free(m_bufferPtr);
		m_bufferPtr = nullptr;
		delete &context;
		m_context = nullptr;
	}

	void CGEVideoDecodeHandler::start()
	{

	}

	void CGEVideoDecodeHandler::end()
	{

	}

	CGEVideoDecodeHandler::NextFrameType CGEVideoDecodeHandler::queryNextFrame()
	{
		CGEVideoDecodeContext& context = *(CGEVideoDecodeContext*)m_context;
		int gotFrame, ret = 0;
		while(av_read_frame(context.pFormatCtx, &context.packet) >= 0)
		{  
			if(context.packet.stream_index == context.videoStreamIndex)
			{  
				ret = avcodec_decode_video2(context.pVideoCodecCtx, context.pVideoFrame, &gotFrame, &context.packet);

// 				if(ret < 0)
// 				{
// 					CGE_LOG_ERROR("Error decoding video frame!\n");
// 					return false;
// 				}

				if(gotFrame)
				{
					AVRational frameRate = av_guess_frame_rate(context.pFormatCtx, context.pVideoStream, context.pVideoFrame);
					m_currentTimestamp += 1000.0 / av_q2d(frameRate);
					av_free_packet(&context.packet);

					memcpy(m_cachedVideoFrame.linesize, context.pVideoFrame->linesize, sizeof(context.pVideoFrame->linesize));
					memcpy(m_cachedVideoFrame.data, context.pVideoFrame->data, sizeof(context.pVideoFrame->data));
					m_cachedVideoFrame.timestamp = av_frame_get_best_effort_timestamp(context.pVideoFrame);
					m_cachedVideoFrame.width = context.pVideoFrame->width;
					m_cachedVideoFrame.height = context.pVideoFrame->height;					
					m_cachedVideoFrame.format = (CGEVideoFormat)context.pVideoFrame->format;					
					return FrameType_VideoFrame;
				}  
			}
			else if(context.packet.stream_index == context.audioStreamIndex)
			{
				ret = avcodec_decode_audio4(context.pAudioCodecCtx, context.pAudioFrame, &gotFrame, &context.packet);

// 				if(ret < 0)
// 				{
// 					CGE_LOG_ERROR("Error decoding audio frame!\n");
// 					return false;
// 				}

				ret = FFMIN(ret, context.packet.size);

				if(gotFrame)
				{
					av_free_packet(&context.packet);

// 					size_t unpaddedLinesize = context.pAudioFrame->nb_samples * av_get_bytes_per_sample((AVSampleFormat)context.pAudioFrame->format);
					m_cachedAudioFrame.timestamp = av_frame_get_best_effort_timestamp(context.pAudioFrame);
					m_cachedAudioFrame.data = context.pAudioFrame->data[0];
					m_cachedAudioFrame.nbSamples = context.pAudioFrame->nb_samples;
					m_cachedAudioFrame.bytesPerSample = av_get_bytes_per_sample((AVSampleFormat)context.pAudioFrame->format);
					m_cachedAudioFrame.channels = av_frame_get_channels(context.pAudioFrame);
					m_cachedAudioFrame.linesize = context.pAudioFrame->linesize[0];					
					m_cachedAudioFrame.format = (CGESampleFormat)context.pAudioFrame->format;
					
					
					return FrameType_AudioFrame;
				}
			}

			av_free_packet(&context.packet);
		}
		return FrameType_NoFrame;
	}

	const CGEVideoFrameBufferData* CGEVideoDecodeHandler::getNextVideoFrame()
	{
		NextFrameType type;

		while ((type = queryNextFrame()) != FrameType_VideoFrame)
		{
			if(type == FrameType_NoFrame)
				return nullptr;
		}

		return &m_cachedVideoFrame;
	}

	const CGEVideoFrameBufferData* CGEVideoDecodeHandler::getCurrentVideoFrame()
	{
		return &m_cachedVideoFrame;
	}

	const CGEAudioFrameBufferData* CGEVideoDecodeHandler::getNextAudio()
	{
		NextFrameType type;

		while ((type = queryNextFrame()) != FrameType_AudioFrame)
		{
			if(type == FrameType_NoFrame)
				return nullptr;
		}

		return &m_cachedAudioFrame;
	}

	const CGEAudioFrameBufferData* CGEVideoDecodeHandler::getCurrentAudioFrame()
	{
		return &m_cachedAudioFrame;
	}

	CGEVideoFrameBufferData CGEVideoDecodeHandler::getNextVideoFrameRGB()
	{
		if(queryNextFrame() != FrameType_VideoFrame)
		{
			return CGEVideoFrameBufferData();
		}
		return getCurrentVideoFrameRGB();
	}

	CGEVideoFrameBufferData CGEVideoDecodeHandler::getCurrentVideoFrameRGB()
	{
		CGEVideoDecodeContext& context = *(CGEVideoDecodeContext*)m_context;
		SwsContext* img_convert_ctx = sws_getContext(context.pVideoCodecCtx->width, context.pVideoCodecCtx->height, context.pVideoCodecCtx->pix_fmt, context.pVideoCodecCtx->width, context.pVideoCodecCtx->height, PIX_FMT_RGBA, m_samplingStyle, nullptr, nullptr, nullptr);  
		// Convert the image from its native format to RGB  
		sws_scale(img_convert_ctx, context.pVideoFrame->data, context.pVideoFrame->linesize, 0, m_height, context.pVideoFrameRGB->data, context.pVideoFrameRGB->linesize);

		CGEVideoFrameBufferData data;
		data.width = m_width;
		data.height = m_height;
		data.format = (CGEVideoFormat)context.pVideoFrameRGB->format;
		memcpy(data.linesize, context.pVideoFrameRGB->linesize, sizeof(context.pVideoFrameRGB->linesize));
		memcpy(data.data, context.pVideoFrameRGB->data, sizeof(context.pVideoFrameRGB->data));
		return data;
	}

	double CGEVideoDecodeHandler::getTotalTime()
	{
		CGEVideoDecodeContext& context = *(CGEVideoDecodeContext*)m_context;
		return (double)context.pFormatCtx->duration;
	}

	double CGEVideoDecodeHandler::getCurrentTimestamp()
	{
		return m_currentTimestamp;
	}

	struct CGEEncoderContextH264
	{
		CGEEncoderContextH264() : pCodecCtx(nullptr), pCodec(nullptr), pFrame(nullptr), pStream(nullptr) {}
		~CGEEncoderContextH264()
		{
			cleanup();
		}

		inline void cleanup()
		{
			avcodec_close(pCodecCtx);
			pCodecCtx = nullptr;
			av_free(pFrame);
			pFrame = nullptr;
		}

		AVCodecContext  *pCodecCtx;
		AVCodec         *pCodec;
		AVFrame         *pFrame;
		AVPacket        packet;
		AVStream        *pStream;
	};

	char CGEVideoEncoderH264::sEndcode[4] = { 0, 0, 1, 0xb7 };

	CGEVideoEncoderH264::CGEVideoEncoderH264() : m_savingFile(nullptr)
	{
		m_context = new CGEEncoderContextH264;
	}

	CGEVideoEncoderH264::~CGEVideoEncoderH264()
	{
		drop();
		delete m_context;
		delete m_savingFile;
	}

	bool CGEVideoEncoderH264::init(const char* filename, int fps, int width, int height)
	{
		char tmpFileName[1024];
		sprintf(tmpFileName, "%s_xx.mp4", filename);
		m_tmpFilename = tmpFileName;
		m_filename = filename;
		if(m_context == nullptr)
			m_context = new CGEEncoderContextH264;

		m_context->pCodec = avcodec_find_encoder(AV_CODEC_ID_H264);
		if(m_context->pCodec == nullptr)
			return false;

		m_context->pCodecCtx = avcodec_alloc_context3(m_context->pCodec);
		if(m_context->pCodecCtx == nullptr)
		{
			m_context->cleanup();
			return false;
		}

		auto& codecContext = m_context->pCodecCtx;

		codecContext->bit_rate = 400000;
		codecContext->width = width;
		codecContext->height = height;
		AVRational avr = {1, fps};
		codecContext->time_base = avr;
		codecContext->gop_size = 10;
		codecContext->max_b_frames = 1;
		codecContext->pix_fmt = AV_PIX_FMT_YUV420P;

		av_opt_set(codecContext->priv_data, "preset", "slow", 0);

		if(avcodec_open2(codecContext, m_context->pCodec, nullptr) < 0)
		{
			m_context->cleanup();
			return false;
		}

		m_savingFile = new std::fstream(filename, std::fstream::out | std::fstream::binary);

		if(!*m_savingFile)
		{
			delete m_savingFile;
			return false;
		}

		auto& frame = m_context->pFrame;
		frame = avcodec_alloc_frame();
		if(!frame)
		{
			m_context->cleanup();
			return false;
		}

		frame->format = codecContext->pix_fmt;
		frame->width = codecContext->width;
		frame->height = codecContext->height;

		int ret = av_image_alloc(frame->data, frame->linesize, codecContext->width, codecContext->height, codecContext->pix_fmt, 32);

		if(ret < 0)
		{
			m_context->cleanup();
			return false;
		}
		return true;
	}

	bool CGEVideoEncoderH264::recordYUV(const void* data, int pts)
	{
		auto& frame = m_context->pFrame;
		int w = frame->width;
		int h = frame->height;
		const unsigned char* buffer = (const unsigned char*)data;
		int stride1 = frame->linesize[0] * h;
		int stride2 = stride1 + frame->linesize[1] * h / 2;
		return recordYUV(buffer, buffer + stride1, buffer + stride2, pts);
	}

	bool CGEVideoEncoderH264::recordYUV(const void* dataY, const void* dataU, const void* dataV, int pts)
	{
		av_init_packet(&m_context->packet);
		auto& packet = m_context->packet;
		packet.data = nullptr;
		packet.size = 0;
		auto& frame = m_context->pFrame;

		//TODO: can be optimized
		memcpy(frame->data[0], dataY, frame->linesize[0] * frame->height);
		memcpy(frame->data[1], dataY, frame->linesize[1] * frame->height);
		memcpy(frame->data[2], dataY, frame->linesize[2] * frame->height);

		frame->pts = pts;

		int gotOutput = 1;

		//		while(gotOutput)

		if(avcodec_encode_video2(m_context->pCodecCtx, &packet, frame, &gotOutput) < 0)
		{
			return false;
		}

		if(gotOutput)
		{
			m_savingFile->write((const char*)packet.data, packet.size);
			av_free_packet(&packet);
		}


		return true;
	}

	bool CGEVideoEncoderH264::recordRGBA(const void* data, int pts)
	{
		av_init_packet(&m_context->packet);
		auto& packet = m_context->packet;
		packet.data = nullptr;
		packet.size = 0;
		auto& frame = m_context->pFrame;

		static SwsContext* img_convert_ctx = nullptr;
		if(img_convert_ctx == nullptr)
			img_convert_ctx = sws_getContext(frame->width, frame->height, AV_PIX_FMT_RGBA, frame->width, frame->height, AV_PIX_FMT_YUV420P, SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);  

		int linesize = frame->width * 4;
		// Convert the image from its native format to YUV
		sws_scale(img_convert_ctx, (unsigned char**)&data, &linesize, 0, frame->height, frame->data, frame->linesize);

		frame->pts = pts;

		int gotOutput;

		if(avcodec_encode_video2(m_context->pCodecCtx, &packet, frame, &gotOutput) < 0)
		{
			return false;
		}

		if(gotOutput)
		{
			m_savingFile->write((const char*)packet.data, packet.size);
			av_free_packet(&packet);
		}

		return true;
	}

	bool CGEVideoEncoderH264::save()
	{
		if(m_savingFile == nullptr)
			return false;

		auto& packet = m_context->packet;

		while(1)
		{
			packet.data = nullptr;
			packet.size = 0;

			int gotOutput;
			if(avcodec_encode_video2(m_context->pCodecCtx, &packet, nullptr, &gotOutput) < 0)
			{
				return false;
			}

			if(!gotOutput)
				break;

			m_savingFile->write((const char*)packet.data, packet.size);
			av_free_packet(&packet);
		}

		m_savingFile->write(sEndcode, sizeof(sEndcode));
		m_savingFile->flush();
		m_savingFile->close();
		delete m_savingFile;
		m_savingFile = nullptr;

		rename(m_tmpFilename.c_str(), m_filename.c_str());

		return true;
	}

	void CGEVideoEncoderH264::drop()
	{
		if(m_savingFile != nullptr)
		{
			m_savingFile->flush();
			m_savingFile->close();
			delete m_savingFile;
			m_savingFile = nullptr;

			remove(m_tmpFilename.c_str());
		}
	}

	//////////////////////////////////////////////////////////////////////////

	struct CGEEncoderContextMP4
	{
		CGEEncoderContextMP4() : pOutputFmt(nullptr), pFormatCtx(nullptr), pVideoStream(nullptr), pAudioStream(nullptr), pVideoCodec(nullptr), pAudioCodec(nullptr), pVideoFrame(nullptr), pAudioFrame(nullptr), pSwsCtx(nullptr), pSwrCtx(nullptr), dstSampleData(nullptr), dstSamplesSize(0), dstSampleDataIndex(0)
		{
			memset(&videoPacket, 0, sizeof(videoPacket));
			memset(&dstPicture, 0, sizeof(dstPicture));
		}
		~CGEEncoderContextMP4()
		{
			cleanup();
		}

		inline void cleanup()
		{
			if(pVideoStream)
			{
				avcodec_close(pVideoStream->codec);
				pVideoStream = nullptr;
			}

			if(pVideoFrame)
			{
				av_free(dstPicture.data[0]);
				av_free(pVideoFrame);
				pVideoFrame = nullptr;
			}

			if(pAudioFrame)
			{
				avcodec_free_frame(&pAudioFrame);
				pAudioFrame = nullptr;
			}

			if(pAudioStream)
			{
				avcodec_close(pAudioStream->codec);
				pAudioStream = nullptr;
			}

			if(pSwrCtx != nullptr)
			{
				av_free(dstSampleData[0]);
				dstSampleData = nullptr;
				dstSamplesLinesize = 0;
				dstSamplesSize = 0;
				swr_free(&pSwrCtx);
				pSwrCtx = nullptr;
			}

			if(pOutputFmt && pFormatCtx && !(pOutputFmt->flags & AVFMT_NOFILE))
			{
				avio_close(pFormatCtx->pb);
			}

			if(pFormatCtx)
			{
				avformat_free_context(pFormatCtx);
				pFormatCtx = nullptr;
			}

			pOutputFmt = nullptr;

			//av_free_packet(&audioPacket);

			memset(&videoPacket, 0, sizeof(videoPacket));
			memset(&dstPicture, 0, sizeof(dstPicture));

			if(pSwsCtx != nullptr)
			{
				sws_freeContext(pSwsCtx);
				pSwsCtx = nullptr;
			}

			pVideoCodec = nullptr;
			pAudioCodec = nullptr;
			dstSampleDataIndex = 0;
		}

		AVOutputFormat  *pOutputFmt;
		AVFormatContext *pFormatCtx;
		AVStream        *pVideoStream;
		AVStream        *pAudioStream;

		AVCodec         *pVideoCodec;
		AVCodec         *pAudioCodec;

		AVFrame         *pVideoFrame;
		AVFrame         *pAudioFrame;
		SwsContext      *pSwsCtx;
		SwrContext      *pSwrCtx;

		AVPacket        videoPacket;
		AVPacket        audioPacket;

		AVPicture       dstPicture;
		uint8_t         **dstSampleData;
		int             dstSampleDataIndex;
		int             dstSamplesLinesize;
		int             dstSamplesSize;

		int             maxDstNbSamples;
	};

	CGEVideoEncoderMP4::CGEVideoEncoderMP4() : m_videoPacketBuffer(nullptr), m_audioPacketBuffer(nullptr)
	{
		m_context = new CGEEncoderContextMP4;
	}

	CGEVideoEncoderMP4::~CGEVideoEncoderMP4()
	{
		drop();
		delete m_context;

		if(m_videoPacketBuffer != nullptr)
			av_free(m_videoPacketBuffer);

		if(m_audioPacketBuffer != nullptr)
			av_free(m_audioPacketBuffer);
	}

	bool CGEVideoEncoderMP4::init(const char* filename, int fps, int width, int height, bool hasAudio)
	{
		m_hasAudio = hasAudio;

		auto ret = avformat_alloc_output_context2(&m_context->pFormatCtx, nullptr, nullptr, filename);

		if(!m_context->pFormatCtx)
		{
			CGE_LOG_INFO("Could not deduce output format from file extension: using MPEG.\n");
			ret = avformat_alloc_output_context2(&m_context->pFormatCtx, NULL, "mp4", filename);
		}

		CGE_LOG_INFO("avformat_alloc_output_context2: ret num %x, filename %s", ret, filename);

		m_filename = filename;

		if(!m_context->pFormatCtx)
		{
			CGE_LOG_ERROR("avformat_alloc_output_context2 failed...");
			return false;
		}

		m_context->pOutputFmt = m_context->pFormatCtx->oformat;
		m_context->pVideoStream = nullptr;

		if(m_context->pOutputFmt->video_codec != AV_CODEC_ID_NONE)
		{
			m_context->pVideoStream = addStream(m_context->pFormatCtx, &m_context->pVideoCodec, m_context->pOutputFmt->video_codec, fps, width, height);
		}

		if(m_hasAudio && m_context->pOutputFmt->audio_codec != AV_CODEC_ID_NONE)
		{
			m_context->pAudioStream = addStream(m_context->pFormatCtx, &m_context->pAudioCodec, m_context->pOutputFmt->audio_codec, fps, width, height);
		}

		if(m_videoPacketBuffer != nullptr)
			av_free(m_videoPacketBuffer);

		if(m_audioPacketBuffer != nullptr)
		{
			av_free(m_audioPacketBuffer);
			m_audioPacketBuffer = nullptr;
		}

		m_videoPacketBufferSize = CGE::CGE_MAX(1024 * 256, width * height * 8);
		m_videoPacketBuffer = (unsigned char*)av_malloc(m_videoPacketBufferSize);

		if(m_hasAudio)
		{
			m_audioPacketBufferSize = 256 * 1024;
			m_audioPacketBuffer = (unsigned char*)av_malloc(m_audioPacketBufferSize);
		}

		//CGE_LOG_INFO("addStream OK!");

		if(!m_context->pVideoStream || !_openVideo())
		{
			CGE_LOG_ERROR("_openVideo failed!\n");
			return false;
		}

		CGE_LOG_INFO("_openVideo OK!");

		if(m_hasAudio)
		{
			if(!m_context->pAudioStream || !_openAudio())
			{
				CGE_LOG_ERROR("_openAudio failed!\n");
				return false;
			}
		}		

		if(!(m_context->pOutputFmt->flags & AVFMT_NOFILE))
		{
			if(0 > avio_open(&m_context->pFormatCtx->pb, filename, AVIO_FLAG_WRITE))
			{
				CGE_LOG_ERROR("could not open file.");
				return false;
			}
		}

		CGE_LOG_INFO("avio_open OK!");

		if(0 > avformat_write_header(m_context->pFormatCtx, nullptr))
		{
			CGE_LOG_ERROR("avformat_write_header failed...");
			return false;
		}

		CGE_LOG_INFO("avformat_write_header OK!");

		if(m_context->pVideoFrame)
			m_context->pVideoFrame->pts = 0;

		return true;
	}

	bool CGEVideoEncoderMP4::_openVideo(/*AVFormatContext *oc, AVCodec *codec, AVStream *st*/)
	{
		int ret;
		AVCodecContext *c = m_context->pVideoStream->codec;
		AVFormatContext *oc = m_context->pFormatCtx;
		AVCodec *codec = m_context->pVideoCodec;

		/* open the codec */
		ret = avcodec_open2(c, codec, nullptr);
		if (ret < 0) {
			//fprintf(stderr, "Could not open video codec: %s\n", av_err2str(ret));
			return false;
		}

		/* allocate and init a re-usable frame */
		m_context->pVideoFrame = avcodec_alloc_frame();
		if (!m_context->pVideoFrame) {
			//fprintf(stderr, "Could not allocate video frame\n");
			return false;
		}

		/* Allocate the encoded raw picture. */

		ret = avpicture_alloc(&m_context->dstPicture, c->pix_fmt, c->width, c->height);
		if (ret < 0)
		{
			// fprintf(stderr, "Could not allocate picture: %s\n", av_err2str(ret));
			return false;
		}

		/* copy data and linesize picture pointers to frame */
		*((AVPicture *)m_context->pVideoFrame) = m_context->dstPicture;
		return true;
	}

	bool CGEVideoEncoderMP4::_openAudio()
	{
		int ret;
		AVCodecContext *c = m_context->pAudioStream->codec;
		AVFormatContext *oc = m_context->pFormatCtx;
		AVCodec *codec = m_context->pAudioCodec;

		AVDictionary *opts = NULL;

		av_dict_set(&opts, "strict", "experimental", 0);

		/* open it */
		ret = avcodec_open2(c, codec, &opts);
		av_dict_free(&opts);

		if (ret < 0)
		{
			CGE_LOG_ERROR("Could not open audio codec: %s\n", av_err2str(ret));
			return false;
		}

		m_context->maxDstNbSamples = c->codec->capabilities & CODEC_CAP_VARIABLE_FRAME_SIZE ?
			10000 : c->frame_size;

		/* create resampler context */
		if (c->sample_fmt != AV_SAMPLE_FMT_S16)
		{
			auto swr_ctx = swr_alloc();
			m_context->pSwrCtx = swr_ctx;
			if (!m_context->pSwrCtx)
			{
				CGE_LOG_ERROR("Could not allocate resampler context\n");
				return false;
			}

			/* set options */
			av_opt_set_int       (swr_ctx, "in_channel_count",   c->channels,       0);
			av_opt_set_int       (swr_ctx, "in_sample_rate",     c->sample_rate,    0);
			av_opt_set_sample_fmt(swr_ctx, "in_sample_fmt",      AV_SAMPLE_FMT_S16, 0);
			av_opt_set_int       (swr_ctx, "out_channel_count",  c->channels,       0);
			av_opt_set_int       (swr_ctx, "out_sample_rate",    c->sample_rate,    0);
			av_opt_set_sample_fmt(swr_ctx, "out_sample_fmt",     c->sample_fmt,     0);

			/* initialize the resampling context */
			if ((ret = swr_init(swr_ctx)) < 0)
			{
				CGE_LOG_ERROR("Failed to initialize the resampling context\n");
				return false;
			}

			/* compute the number of converted samples: buffering is avoided
			* ensuring that the output data will contain at least all the
			* converted input samples */
			ret = av_samples_alloc_array_and_samples(&m_context->dstSampleData, &m_context->dstSamplesLinesize, c->channels, m_context->maxDstNbSamples, c->sample_fmt, 0);
			if (ret < 0)
			{
				CGE_LOG_ERROR("Could not allocate destination samples\n");
				return false;
			}

			m_context->dstSamplesSize = av_samples_get_buffer_size(NULL, c->channels, m_context->maxDstNbSamples, c->sample_fmt, 0);
		}

		return true;
	}

	bool CGEVideoEncoderMP4::record(const ImageData& data)
	{
		AVPixelFormat pixFmt = (AVPixelFormat)m_recordDataFmt;
		AVCodecContext *codecCtx = m_context->pVideoStream->codec;

		// auto tm = getCurrentTimeMillis(), tm2 = tm;

		if(data.data[0] != nullptr)
		{
			if(pixFmt != codecCtx->pix_fmt || data.width != codecCtx->width || data.height != codecCtx->height)
			{
				if(!m_context->pSwsCtx)
				{
					m_context->pSwsCtx = sws_getContext(data.width, data.height, pixFmt, 
						codecCtx->width, codecCtx->height, codecCtx->pix_fmt, SWS_POINT, nullptr, nullptr, nullptr);
					if(!m_context->pSwsCtx)
					{
						CGE_LOG_ERROR("create sws context failed!");
						return false;
					}
					CGE_LOG_INFO("create sws context success!");
				}

				sws_scale(m_context->pSwsCtx, data.data, data.linesize, 0, codecCtx->height, m_context->dstPicture.data, m_context->dstPicture.linesize);
			}
			else
			{
				const int sz1 = sizeof(m_context->dstPicture.data) < sizeof(data.data) ? sizeof(m_context->dstPicture.data) : sizeof(data.data);
				const int sz2 = sizeof(m_context->dstPicture.linesize) < sizeof(data.linesize) ? sizeof(m_context->dstPicture.linesize) : sizeof(data.linesize);

				memcpy(m_context->dstPicture.data, data.data, sz1);
				memcpy(m_context->dstPicture.linesize, data.linesize, sz2);
			}

			m_context->pVideoFrame->pts = data.pts;
		}
		else
		{
			//++m_context->pVideoFrame->pts;
		}

		// tm2 = getCurrentTimeMillis();
		// CGE_LOG_ERROR("Format conversion time: %g", (tm2 - tm));
		// tm = tm2;

		if(m_context->pFormatCtx->oformat->flags & AVFMT_RAWPICTURE)
		{
			AVPacket& pkt = m_context->videoPacket;
			av_init_packet(&pkt);

			pkt.flags |= AV_PKT_FLAG_KEY;
			pkt.stream_index = m_context->pVideoStream->index;
			pkt.data = m_context->dstPicture.data[0];
			pkt.size = sizeof(AVPicture);

			m_mutex.lock();
			auto ret =  av_interleaved_write_frame(m_context->pFormatCtx, &pkt);
			m_mutex.unlock();

			if(0 > ret)
			{
				CGE_LOG_ERROR("av_interleaved_write_frame error1...");
				return false;
			}
			av_free_packet(&pkt);
		}
		else
		{
			AVPacket& pkt = m_context->videoPacket;
			int gotPacket;
			av_init_packet(&pkt);
			pkt.data = m_videoPacketBuffer;
			pkt.size = m_videoPacketBufferSize;

			//avcodec_encode_video2 is a slow operation
			if(0 > avcodec_encode_video2(codecCtx, &pkt, data.data[0] == nullptr ? nullptr : m_context->pVideoFrame, &gotPacket))
			{
				CGE_LOG_ERROR("avcodec_encode_video2 error...\n");
				return false;
			}

			// tm2 = getCurrentTimeMillis();
			// CGE_LOG_ERROR("Encode time: %g", (tm2 - tm));
			// tm = tm2;

			if(gotPacket && pkt.size)
			{
				// CGE_LOG_ERROR("PTS %d, %d\n", data.pts, pkt.pts);

				if(pkt.pts != AV_NOPTS_VALUE)
				{
					pkt.pts = av_rescale_q(pkt.pts, codecCtx->time_base, m_context->pVideoStream->time_base);
				}
				if(pkt.dts != AV_NOPTS_VALUE)
				{
					pkt.dts = av_rescale_q(pkt.dts, codecCtx->time_base, m_context->pVideoStream->time_base);
				}

				pkt.stream_index = m_context->pVideoStream->index;

				m_mutex.lock();
				auto ret = av_interleaved_write_frame(m_context->pFormatCtx, &pkt);
				m_mutex.unlock();

				// av_free_packet(&pkt);

				// tm2 = getCurrentTimeMillis();
				// CGE_LOG_ERROR("av_interleaved_write_frame time: %g", (tm2 - tm));
				// tm = tm2;	

				if(0 > ret)
				{
					CGE_LOG_ERROR("av_interleaved_write_frame error2... 0x%x\n", ret);
					return false;
				}

			}
			else if(data.data[0] == nullptr)
			{
				return false;
			}
		}

		return m_context->pVideoFrame->key_frame != 0;
	}

	//Cache audio data; can only store multiples of 1024 at a time
	bool CGEVideoEncoderMP4::record(const AudioSampleData& data)
	{
		assert(m_hasAudio);

		if(data.data[0] != nullptr)
		{
//			assert(m_context->pSwrCtx != nullptr); // Resampling is always required

			if(m_context->pAudioFrame == nullptr)
			{
				m_context->pAudioFrame = avcodec_alloc_frame();
			}

			AVCodecContext* audioCodec = m_context->pAudioStream->codec;
			AVFrame* pAudioFrame = m_context->pAudioFrame;

			if(m_context->pSwrCtx == nullptr)
			{
				m_context->dstSampleDataIndex = 0;
				pAudioFrame->nb_samples = data.nbSamples[0];
				pAudioFrame->quality = audioCodec->global_quality;
				avcodec_fill_audio_frame(pAudioFrame, audioCodec->channels, audioCodec->sample_fmt, data.data[0], data.nbSamples[0] * 2, 0);
				pAudioFrame->data[0] = (uint8_t*)data.data[0];
				pAudioFrame->linesize[0] = data.nbSamples[0] * 2;
				record(pAudioFrame);
			}
			else
			{				
				int srcNbSamples = data.nbSamples[0];

				while(1)
				{
					int dstNbSamples = m_context->maxDstNbSamples - m_context->dstSampleDataIndex;

					// 4 stands for the output channels.
					auto convertData = m_context->dstSampleData[0] + 4 * m_context->dstSampleDataIndex;

					int ret = swr_convert(m_context->pSwrCtx, &convertData, dstNbSamples, (const uint8_t**)data.data, srcNbSamples);

					if(ret == 0)
					{
						// CGE_LOG_ERROR("ret == 0");
						break;
					}
					else if(ret < 0)
					{
						CGE_LOG_ERROR("Error while converting...\n");
						return false;
					}

					m_context->dstSampleDataIndex += ret;
					srcNbSamples = 0;

					if(m_context->dstSampleDataIndex >= m_context->maxDstNbSamples)
					{
						m_context->dstSampleDataIndex = 0;
						// CGE_LOG_ERROR("Recording...");
						pAudioFrame->nb_samples = m_context->maxDstNbSamples;
						pAudioFrame->quality = audioCodec->global_quality;
						avcodec_fill_audio_frame(pAudioFrame, audioCodec->channels, audioCodec->sample_fmt, m_context->dstSampleData[0], m_context->dstSamplesSize, 0);
						pAudioFrame->data[0] = m_context->dstSampleData[0];
						pAudioFrame->linesize[0] = m_context->dstSamplesSize;
						record(pAudioFrame);
					}
				}
			}

			return pAudioFrame->key_frame != 0;
		}

		return record(nullptr);
	}

	bool CGEVideoEncoderMP4::record(AVFrame* pAudioFrame)
	{
		assert(m_hasAudio);

		AVPacket pkt = { 0 };
		av_init_packet(&pkt);
		pkt.data = m_audioPacketBuffer;
		pkt.size = m_audioPacketBufferSize;

		int gotPacket;

		int ret = avcodec_encode_audio2(m_context->pAudioStream->codec, &pkt, pAudioFrame, &gotPacket);

		if(ret < 0)
		{
			CGE_LOG_ERROR("Error encoding audio frame: %s\n", av_err2str(ret));
			return false;
		}

		if(!gotPacket || pkt.size == 0)
			return false;

		pkt.stream_index = m_context->pAudioStream->index;

		m_mutex.lock();
		ret = av_interleaved_write_frame(m_context->pFormatCtx, &pkt);
		m_mutex.unlock();

		// av_free_packet(&pkt);

		if(ret != 0)
		{
			CGE_LOG_ERROR("Error while writing audio frame: %s\n", av_err2str(ret));
			return false;
		}

		return m_context->pAudioFrame->key_frame != 0;
	}

	bool CGEVideoEncoderMP4::save()
	{
		{
			ImageData videoData = {0};
			while(record(videoData));
		}

		if(m_hasAudio)
		{
			AudioSampleData audioData = {0};
			while(record(audioData));
		}

		if(0 != av_write_trailer(m_context->pFormatCtx))
			return false;
		m_context->cleanup();
		return true;
	}

	void CGEVideoEncoderMP4::drop()
	{
		m_context->cleanup();
	}

	int CGEVideoEncoderMP4::_queryDataFormat(RecordDataFormat fmt)
	{
		AVPixelFormat result = AV_PIX_FMT_NONE;
		switch (fmt)
		{
		case FMT_RGBA8888:
			result = AV_PIX_FMT_RGBA;
			break;
		case FMT_RGB565:
			result = AV_PIX_FMT_RGB565;
			break;
		case FMT_BGR24:
			result = AV_PIX_FMT_BGR24;
			break;
		case FMT_GRAY8:
			result = AV_PIX_FMT_GRAY8;
			break;
		case FMT_NV21:
			result = AV_PIX_FMT_NV21;
			break;
		case FMT_YUV420P:
			result = AV_PIX_FMT_YUV420P;
			break;
		default:
			break;
		}
		return result;
	}

	void CGEVideoEncoderMP4::setRecordDataFormat(RecordDataFormat fmt)
	{
		m_recordDataFmt = _queryDataFormat(fmt);
	}

	double CGEVideoEncoderMP4::getVideoStreamTime()
	{
		return (m_context && m_context->pVideoStream) ? m_context->pVideoStream->pts.val * av_q2d(m_context->pVideoStream->time_base) : 0.0;
	}

	double CGEVideoEncoderMP4::getAudioStreamTime()
	{
		return (m_context && m_context->pAudioStream) ? m_context->pAudioStream->pts.val * av_q2d(m_context->pAudioStream->time_base) : 0.0;
	}
}

#endif