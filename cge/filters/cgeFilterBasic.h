/*
 * cgeFilterBasic.h
 *
 *  Created on: 2013-12-25
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef _CGEBASICADJUST_H_
#define _CGEBASICADJUST_H_

#include "cgeBrightnessAdjust.h"
#include "cgeContrastAdjust.h"
#include "cgeSharpenBlurAdjust.h"
#include "cgeSaturationAdjust.h"
#include "cgeShadowHighlightAdjust.h"
#include "cgeWhiteBalanceAdjust.h"
#include "cgeMonochromeAdjust.h"
#include "cgeCurveAdjust.h"
#include "cgeColorLevelAdjust.h"
#include "cgeVignetteAdjust.h"
#include "cgeTiltshiftAdjust.h"
#include "cgeZoomBlurAdjust.h"
#include "cgeMotionBlurAdjust.h"
#include "cgeSelectiveColorAdjust.h"
#include "cgeExposureAdjust.h"
#include "cgeHueAdjust.h"
#include "cgeFastTiltShift.h"
#include "cgeEdgeExtract.h"
#include "cgeColorBalanceAdjust.h"
#include "cgeLookupFilter.h"

// Factory function declarations — always use CGEImageFilterInterface* return
// type to ensure consistent symbol mangling across all compilers (MSVC includes
// the return type in mangled names, unlike GCC/Clang).
namespace CGE
{
CGEImageFilterInterface* createBrightnessFilter();
CGEImageFilterInterface* createBrightnessFastFilter();
CGEImageFilterInterface* createContrastFilter();
CGEImageFilterInterface* createSharpenBlurFilter();
CGEImageFilterInterface* createSharpenBlurFastFilter();
CGEImageFilterInterface* createSharpenBlurSimpleFilter();
CGEImageFilterInterface* createSharpenBlurSimpleBetterFilter();
CGEImageFilterInterface* createSaturationHSLFilter();
CGEImageFilterInterface* createSaturationFilter();
CGEImageFilterInterface* createShadowHighlightFilter();
CGEImageFilterInterface* createShadowHighlightFastFilter();
CGEImageFilterInterface* createWhiteBalanceFilter();
CGEImageFilterInterface* createWhiteBalanceFastFilter();
CGEImageFilterInterface* createMonochromeFilter();
CGEImageFilterInterface* createCurveTexFilter();
CGEImageFilterInterface* createCurveFilter();
CGEImageFilterInterface* createMoreCurveFilter();
CGEImageFilterInterface* createMoreCurveTexFilter();
CGEImageFilterInterface* createColorLevelFilter();
CGEImageFilterInterface* createVignetteFilter();
CGEImageFilterInterface* createVignetteExtFilter();
CGEImageFilterInterface* createTiltshiftVectorFilter();
CGEImageFilterInterface* createTiltshiftEllipseFilter();
CGEImageFilterInterface* createFixedTiltshiftVectorFilter();
CGEImageFilterInterface* createFixedTiltshiftEllipseFilter();
CGEImageFilterInterface* createSharpenBlurFastWithFixedBlurRadiusFilter();
CGEImageFilterInterface* createZoomBlurFilter();
CGEImageFilterInterface* createZoomBlur2Filter();
CGEImageFilterInterface* createMotionBlurFilter();
CGEImageFilterInterface* createSelectiveColorFilter();
CGEImageFilterInterface* createExposureFilter();
CGEImageFilterInterface* createHueAdjustFilter();
CGEImageFilterInterface* createColorBalanceFilter();
CGEImageFilterInterface* createLookupFilter();
}  // namespace CGE

#endif
