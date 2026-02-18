/*
 * cgeExtends.h
 *
 *  Created on: 2014-9-18
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef _CGE_EXTENDS_H_
#define _CGE_EXTENDS_H_

#ifndef CGE_EXT_NO_VIDEO

#include "video/cgeVideoPlayer.h"
#include "video/cgeVideoEncoder.h"
#include "video/cgeVideoDecoder.h"
#include "video/cgeVideoUtils.h"

#endif

#ifndef CGE_EXT_NO_THREAD
#include "thread/cgeThread.h"
#endif

namespace CGE
{
bool cgeInitExtends();
}

#endif
