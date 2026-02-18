/*
 * cgeGeometryUtils.h
 *
 *  Created on: 2017-4-15
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef _CGE_GEOMETRYUTILS_H_
#define _CGE_GEOMETRYUTILS_H_

#include "cgeGLFunctions.h"
#include "cgeVec.h"

namespace CGE
{
	class CGEPointDrawer
	{
	public:

		CGE_COMMON_CREATE_FUNC(CGEPointDrawer, init);

		virtual ~CGEPointDrawer() {}

		void draw(Vec2f* points, int count, int stride = 0);

		void setPointSize(float pointSize);
		void setColor(Vec4f color);

	protected:

		CGEPointDrawer();
		bool init();

		virtual CGEConstString getVSH();
		virtual CGEConstString getFSH();

		ProgramObject m_program;
		GLuint m_pointSizeLoc, m_colorLoc;
	};


}


#endif // !_CGE_GEOMETRYUTILS_H_
