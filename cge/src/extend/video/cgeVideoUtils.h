/*
* cgeVideoUtils.h
*
*  Created on: 2015-12-10
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#include "cgeGLFunctions.h"

namespace CGE
{
	//bool cgeGenerateVideoWithFilter(const char* outputFilename, const char* inputFilename);

	class CGEFastFrameHandler : public CGEImageHandler
	{
	public:

		void processingFilters();

		void swapBufferFBO();

		inline void useImageFBO()
		{
			CGEImageHandler::useImageFBO();
		}
	};

	bool cgeGenerateVideoWithFilter(const char* outputFilename, const char* inputFilename, const char* filterConfig, float filterIntensity, GLuint texID, CGETextureBlendMode blendMode, float blendIntensity);

}