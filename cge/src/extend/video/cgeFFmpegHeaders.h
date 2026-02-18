/*
* cgeFFmpegHeaders.h
*
*  Created on: 2015-12-10
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

// Do not include directly in header files, as it would create a heavy dependency on ffmpeg headers

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"

#include "libavutil/opt.h"
#include "libavutil/imgutils.h"

}