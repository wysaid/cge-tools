/*
 * cgeZoomBlurAdjust.h
 *
 *  Created on: 2014-9-17
 *      Author: wxfred
 */

#ifndef _CGEZOOMBLURADJUST_H
#define _CGEZOOMBLURADJUST_H

#include "../../core/cgeGLFunctions.h"

namespace CGE
{
// Suitable for outward-spreading cases
class CGEZoomBlurFilter : public CGEImageFilterInterface
{
public:
    CGEZoomBlurFilter() {}
    ~CGEZoomBlurFilter() {}

    void setCenter(float x, float y);   // texture coordinate
    void setIntensity(float strength);  // range: [0, 1]

    bool init();

protected:
    static CGEConstString paramCenterName;
    static CGEConstString paramIntensityName;
};

// Compromise approach, handles both outward and inward spreading
class CGEZoomBlur2Filter : public CGEZoomBlurFilter
{
public:
    CGEZoomBlur2Filter() {}
    ~CGEZoomBlur2Filter() {}

    bool init();

protected:
    static CGEConstString paramStepsName;
};

}  // namespace CGE

#endif