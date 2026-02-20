/*
 * cgeFastTiltShift.h
 *
 *  Created on: 2015-01-12
 *      Author: Su Zhiyun
 */

#ifndef _CGEEDGEEXTRACT_H_
#define _CGEEDGEEXTRACT_H_

#include "cgeGLFunctions.h"
#include "cgeImageFilter.h"

namespace CGE
{
class CGEEdgeExtractInterface : public CGEImageFilterInterface
{
public:
    CGEEdgeExtractInterface() {}
    ~CGEEdgeExtractInterface() {}

    virtual bool init() = 0;


    virtual void setEdgeSaturation(float sat = 1.0) = 0;


    virtual void setEdgeMinMax(float emin = 0.0, float emax = 1.0) = 0;


    virtual void setEdgeLevel(int level = -1) = 0;


    static CGEEdgeExtractInterface* Create();
};

// Avoid redeclaration when filters.h (public API) is already included
#ifndef _CGE_FILTERS_H_
CGEImageFilterInterface* createEdgeExtractFilter();
#endif
}  // namespace CGE

#endif