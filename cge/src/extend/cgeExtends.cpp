/*
* cgeExtends.cpp
*
*  Created on: 2014-9-18
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#include "cgeExtends.h"

extern "C"
{
	void av_register_all();
};

namespace CGE
{
	bool cgeInitExtends()
	{
#ifndef _CGE_EXT_NO_VIDEO_
		
		av_register_all();
#endif		
		return true;
	}
}