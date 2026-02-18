/*
* cgeVideoExt.h
*
*  Created on: 2014-9-18
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#if !(defined(_CGE_VIDEOEXT_H_) || defined(_CGE_EXT_NO_VIDEO_))
#define _CGE_VIDEOEXT_H_

#define CGE_EXT_VIDEO 1

#include <fstream>
#include "thread/cgeThread.h"

struct AVFrame;

namespace CGE
{
	bool cgeInitVideoExt();

	struct CGEVideoDecodeContext;

	enum CGEVideoFormat
	{
		CGE_VIDEO_FORMAT_NONE = -1,
		CGE_VIDEO_FORMAT_YUV420P,
		CGE_VIDEO_FORMAT_YUYV422,
		CGE_VIDEO_FORMAT_RGB24,
		CGE_VIDEO_FORMAT_BGR24,
		CGE_VIDEO_FORMAT_YUV422P,
		CGE_VIDEO_FORMAT_YUV444P,
		CGE_VIDEO_FORMAT_YUV410P,
		CGE_VIDEO_FORMAT_YUV411P,
		CGE_VIDEO_FORMAT_GRAY8,
		CGE_VIDEO_FORMAT_MONOWHITE,
		CGE_VIDEO_FORMAT_MONOBLACK,
		CGE_VIDEO_FORMAT_PAL8,
		CGE_VIDEO_FORMAT_YUVJ420P,
		CGE_VIDEO_FORMAT_YUVJ422P,
		CGE_VIDEO_FORMAT_YUVJ444P,

		CGE_VIDEO_FORMAT_XVMC_MPEG2_MC,
		CGE_VIDEO_FORMAT_XVMC_MPEG2_IDCT,
		CGE_VIDEO_FORMAT_UYVY422,
		CGE_VIDEO_FORMAT_UYYVYY411,
		CGE_VIDEO_FORMAT_BGR8,
		CGE_VIDEO_FORMAT_BGR4,
		CGE_VIDEO_FORMAT_BGR4_BYTE,
		CGE_VIDEO_FORMAT_RGB8,
		CGE_VIDEO_FORMAT_RGB4,
		CGE_VIDEO_FORMAT_RGB4_BYTE,
		CGE_VIDEO_FORMAT_NV12,
		CGE_VIDEO_FORMAT_NV21,

		CGE_VIDEO_FORMAT_ARGB,
		CGE_VIDEO_FORMAT_RGBA,
		CGE_VIDEO_FORMAT_ABGR,
		CGE_VIDEO_FORMAT_BGRA,

		CGE_VIDEO_FORMAT_GRAY16BE,
		CGE_VIDEO_FORMAT_GRAY16LE,
		CGE_VIDEO_FORMAT_YUV440P,
		CGE_VIDEO_FORMAT_YUVJ440P,
		CGE_VIDEO_FORMAT_YUVA420P,
		CGE_VIDEO_FORMAT_VDPAU_H264,
		CGE_VIDEO_FORMAT_VDPAU_MPEG1,
		CGE_VIDEO_FORMAT_VDPAU_MPEG2,
		CGE_VIDEO_FORMAT_VDPAU_WMV3,
		CGE_VIDEO_FORMAT_VDPAU_VC1,

		CGE_VIDEO_FORMAT_RGB48BE,
		CGE_VIDEO_FORMAT_RGB48LE,

		CGE_VIDEO_FORMAT_RGB565BE,
		CGE_VIDEO_FORMAT_RGB565LE,
		CGE_VIDEO_FORMAT_RGB555BE,
		CGE_VIDEO_FORMAT_RGB555LE,

		CGE_VIDEO_FORMAT_BGR565BE,
		CGE_VIDEO_FORMAT_BGR565LE,
		CGE_VIDEO_FORMAT_BGR555BE,
		CGE_VIDEO_FORMAT_BGR555LE,

		CGE_VIDEO_FORMAT_VAAPI_MOCO,
		CGE_VIDEO_FORMAT_VAAPI_IDCT,
		CGE_VIDEO_FORMAT_VAAPI_VLD,

		CGE_VIDEO_FORMAT_YUV420P16LE,
		CGE_VIDEO_FORMAT_YUV420P16BE,
		CGE_VIDEO_FORMAT_YUV422P16LE,
		CGE_VIDEO_FORMAT_YUV422P16BE,
		CGE_VIDEO_FORMAT_YUV444P16LE,
		CGE_VIDEO_FORMAT_YUV444P16BE,
		CGE_VIDEO_FORMAT_VDPAU_MPEG4,

		CGE_VIDEO_FORMAT_DXVA2_VLD,

		CGE_VIDEO_FORMAT_RGB444LE,
		CGE_VIDEO_FORMAT_RGB444BE,
		CGE_VIDEO_FORMAT_BGR444LE,
		CGE_VIDEO_FORMAT_BGR444BE,
		CGE_VIDEO_FORMAT_YA8,

		CGE_VIDEO_FORMAT_Y400A = CGE_VIDEO_FORMAT_YA8,
		CGE_VIDEO_FORMAT_GRAY8A= CGE_VIDEO_FORMAT_YA8,

		CGE_VIDEO_FORMAT_BGR48BE,
		CGE_VIDEO_FORMAT_BGR48LE,

		CGE_VIDEO_FORMAT_YUV420P9BE,
		CGE_VIDEO_FORMAT_YUV420P9LE,
		CGE_VIDEO_FORMAT_YUV420P10BE,
		CGE_VIDEO_FORMAT_YUV420P10LE,
		CGE_VIDEO_FORMAT_YUV422P10BE,
		CGE_VIDEO_FORMAT_YUV422P10LE,
		CGE_VIDEO_FORMAT_YUV444P9BE,
		CGE_VIDEO_FORMAT_YUV444P9LE,
		CGE_VIDEO_FORMAT_YUV444P10BE,
		CGE_VIDEO_FORMAT_YUV444P10LE,
		CGE_VIDEO_FORMAT_YUV422P9BE,
		CGE_VIDEO_FORMAT_YUV422P9LE,
		CGE_VIDEO_FORMAT_VDA_VLD,

		CGE_VIDEO_FORMAT_RGBA64BE,
		CGE_VIDEO_FORMAT_RGBA64LE,
		CGE_VIDEO_FORMAT_BGRA64BE,
		CGE_VIDEO_FORMAT_BGRA64LE,

		CGE_VIDEO_FORMAT_GBRP,
		CGE_VIDEO_FORMAT_GBRP9BE,
		CGE_VIDEO_FORMAT_GBRP9LE,
		CGE_VIDEO_FORMAT_GBRP10BE,
		CGE_VIDEO_FORMAT_GBRP10LE,
		CGE_VIDEO_FORMAT_GBRP16BE,
		CGE_VIDEO_FORMAT_GBRP16LE,

		CGE_VIDEO_FORMAT_YUVA422P_LIBAV,
		CGE_VIDEO_FORMAT_YUVA444P_LIBAV,

		CGE_VIDEO_FORMAT_YUVA420P9BE,
		CGE_VIDEO_FORMAT_YUVA420P9LE,
		CGE_VIDEO_FORMAT_YUVA422P9BE,
		CGE_VIDEO_FORMAT_YUVA422P9LE,
		CGE_VIDEO_FORMAT_YUVA444P9BE,
		CGE_VIDEO_FORMAT_YUVA444P9LE,
		CGE_VIDEO_FORMAT_YUVA420P10BE,
		CGE_VIDEO_FORMAT_YUVA420P10LE,
		CGE_VIDEO_FORMAT_YUVA422P10BE,
		CGE_VIDEO_FORMAT_YUVA422P10LE,
		CGE_VIDEO_FORMAT_YUVA444P10BE,
		CGE_VIDEO_FORMAT_YUVA444P10LE,
		CGE_VIDEO_FORMAT_YUVA420P16BE,
		CGE_VIDEO_FORMAT_YUVA420P16LE,
		CGE_VIDEO_FORMAT_YUVA422P16BE,
		CGE_VIDEO_FORMAT_YUVA422P16LE,
		CGE_VIDEO_FORMAT_YUVA444P16BE,
		CGE_VIDEO_FORMAT_YUVA444P16LE,

		CGE_VIDEO_FORMAT_VDPAU,

		CGE_VIDEO_FORMAT_XYZ12LE,
		CGE_VIDEO_FORMAT_XYZ12BE,
		CGE_VIDEO_FORMAT_NV16,
		CGE_VIDEO_FORMAT_NV20LE,
		CGE_VIDEO_FORMAT_NV20BE,

		CGE_VIDEO_FORMAT_RGBA64BE_LIBAV,
		CGE_VIDEO_FORMAT_RGBA64LE_LIBAV,
		CGE_VIDEO_FORMAT_BGRA64BE_LIBAV,
		CGE_VIDEO_FORMAT_BGRA64LE_LIBAV,

		CGE_VIDEO_FORMAT_YVYU422,
		CGE_VIDEO_FORMAT_VDA,
		CGE_VIDEO_FORMAT_YA16BE,
		CGE_VIDEO_FORMAT_YA16LE,

	};

	enum CGESampleFormat
	{
		CGE_SAMPLE_FMT_NONE = -1,
		CGE_SAMPLE_FMT_U8,          ///< unsigned 8 bits
		CGE_SAMPLE_FMT_S16,         ///< signed 16 bits
		CGE_SAMPLE_FMT_S32,         ///< signed 32 bits
		CGE_SAMPLE_FMT_FLT,         ///< float
		CGE_SAMPLE_FMT_DBL,         ///< double

		CGE_SAMPLE_FMT_U8P,         ///< unsigned 8 bits, planar
		CGE_SAMPLE_FMT_S16P,        ///< signed 16 bits, planar
		CGE_SAMPLE_FMT_S32P,        ///< signed 32 bits, planar
		CGE_SAMPLE_FMT_FLTP,        ///< float, planar
		CGE_SAMPLE_FMT_DBLP,        ///< double, planar

		CGE_SAMPLE_FMT_NB           ///< Number of sample formats. DO NOT USE if linking dynamically
	};

	struct CGEVideoFrameBufferData
	{
		const unsigned char* data[8]; // Shallow copy; actual content is an ffmpeg frame
		int linesize[8];
		double timestamp;
		int width;
		int height;		
		CGEVideoFormat format;
		
	};

	struct CGEAudioFrameBufferData
	{
		double timestamp;
		const unsigned char* data; // Shallow copy; actual content is an ffmpeg frame
		int nbSamples;		//Number of samples
		int bytesPerSample; //Actual data size = nbSamples * bytesPerSample
		int channels;		//Number of audio channels
		int linesize;		//Size including all padding data
		CGESampleFormat format;		
	};

	class CGEVideoDecodeHandler
	{
	public:
		CGEVideoDecodeHandler();
		~CGEVideoDecodeHandler();

		enum SamplingStyle { ssFastBilinear = 1, ssBilinear = 2, ssBicubic = 4, ssX = 8, ssPoint = 0x10, ssArea = 0x20, ssBicublin = 0x40, ssGauss = 0x80, ssSinc = 0x100, ssLanczos = 0x200, ssSpline = 0x400 };

		enum NextFrameType{ FrameType_NoFrame, FrameType_VideoFrame, FrameType_AudioFrame };

		bool initFrameRGB();

		bool open(const char* filename);
		void close();

		void start();
		void end();

		NextFrameType queryNextFrame();

		const CGEVideoFrameBufferData* getNextVideoFrame(); //Skip audio frames and get the next video frame
		const CGEVideoFrameBufferData* getCurrentVideoFrame(); //Get the current frame; repeated calls may return the same frame

		const CGEAudioFrameBufferData* getNextAudio();
		const CGEAudioFrameBufferData* getCurrentAudioFrame();

		CGEVideoFrameBufferData getNextVideoFrameRGB();  //Format is RGBA if withAlpha is true
		CGEVideoFrameBufferData getCurrentVideoFrameRGB(); //Format is RGBA if withAlpha is true

		void setSamplingStyle(SamplingStyle style) { m_samplingStyle = style; }

		int getWidth() { return m_width; }
		int getHeight() { return m_height; }
		double getTotalTime();
		double getCurrentTimestamp();

	protected:
		void* m_context;
		int m_width, m_height;
		SamplingStyle m_samplingStyle;

		CGEVideoFrameBufferData m_cachedVideoFrame;
		CGEAudioFrameBufferData m_cachedAudioFrame;
		double m_currentTimestamp;

	private:
		unsigned char* m_bufferPtr; //Video frame buffer
	};

	struct CGEEncoderContextH264;

	//Can only generate raw H264 video stream
	class CGEVideoEncoderH264
	{
	public:
		CGEVideoEncoderH264();
		~CGEVideoEncoderH264();

		bool init(const char* filename, int fps, int width, int height);

		//Minimal functionality: data must be in YUV420P format
		//and size must match the dimensions given at initialization
		bool recordYUV(const void* data, int pts);
		bool recordYUV(const void* dataY, const void* dataU, const void* dataV, int pts);

		//For testing: performs a single conversion of RGB data
		//Alpha channel will be ignored
		bool recordRGBA(const void* data, int pts);

		//Save video
		bool save();

		//Discard video
		void drop();

	protected:
		static char sEndcode[4];

	private: 
		CGEEncoderContextH264* m_context;
		std::fstream* m_savingFile;
		std::string m_filename, m_tmpFilename;
	};

	//////////////////////////////////////////////////////////////////////////


	struct CGEEncoderContextMP4;
	
	// H264 video encoder with FLTP float mono audio, mp4 format
	class CGEVideoEncoderMP4
	{
	public:
		CGEVideoEncoderMP4();
		~CGEVideoEncoderMP4();

		enum RecordDataFormat
		{
			FMT_RGBA8888,
			FMT_RGB565,
			FMT_BGR24,
			FMT_GRAY8,
			FMT_NV21,
			FMT_YUV420P
		};

		struct ImageData
		{
			unsigned char* data[8];
			int width, height;
			int linesize[8];
			long pts;
		};

		struct AudioSampleData
		{
			unsigned short* data[8];
			int nbSamples[8]; //Audio frame size
			int channels; //Number of audio channels
		};

		bool init(const char* filename, int fps, int width, int height, bool hasAudio = true);

		void setRecordDataFormat(RecordDataFormat fmt);

		//Both record methods guarantee thread safety when writing to file
		bool record(const ImageData& data);
		bool record(const AudioSampleData& data);

		bool record(AVFrame*);

		double getVideoStreamTime();
		double getAudioStreamTime();

		//Save video
		bool save();

		//Discard video
		void drop();

	protected:
		bool _openVideo();
		bool _openAudio();
		int _queryDataFormat(RecordDataFormat fmt);

	private:
		CGEEncoderContextMP4* m_context;
		std::string m_filename;
		int m_recordDataFmt;
		std::mutex m_mutex;

		unsigned char* m_videoPacketBuffer;
		int m_videoPacketBufferSize;
		unsigned char* m_audioPacketBuffer;
		int m_audioPacketBufferSize;

		bool m_hasAudio;
	};

}

#endif