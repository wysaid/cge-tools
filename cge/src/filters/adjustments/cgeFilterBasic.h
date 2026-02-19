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
#include "../sharpen/cgeSharpenBlurAdjust.h"
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
#include "../blur/cgeFastTiltShift.h"
#include "../effects/cgeEdgeExtract.h"
#include "cgeColorBalanceAdjust.h"
#include "cgeLookupFilter.h"

// Only declare factory functions with concrete types when the public API header
// (filters.h) has NOT been included. When filters.h is included, it already
// declares these functions as CGEImageFilterInterface*, avoiding redeclaration
// conflicts with different return types.
#ifndef _CGE_FILTERS_H_
namespace CGE
{
CGEBrightnessFilter* createBrightnessFilter();
CGEBrightnessFastFilter* createBrightnessFastFilter();
CGEContrastFilter* createContrastFilter();
CGESharpenBlurFilter* createSharpenBlurFilter();
CGESharpenBlurFastFilter* createSharpenBlurFastFilter();
CGESharpenBlurSimpleFilter* createSharpenBlurSimpleFilter();
CGESharpenBlurSimpleBetterFilter* createSharpenBlurSimpleBetterFilter();
CGESaturationHSLFilter* createSaturationHSLFilter();
CGESaturationFilter* createSaturationFilter();
CGEShadowHighlightFilter* createShadowHighlightFilter();
CGEShadowHighlightFastFilter* createShadowHighlightFastFilter();
CGEWhiteBalanceFilter* createWhiteBalanceFilter();
CGEWhiteBalanceFastFilter* createWhiteBalanceFastFilter();
CGEMonochromeFilter* createMonochromeFilter();  // Monochrome
CGECurveTexFilter* createCurveTexFilter();
CGECurveFilter* createCurveFilter();
CGEMoreCurveFilter* createMoreCurveFilter();
CGEMoreCurveTexFilter* createMoreCurveTexFilter();
CGEColorLevelFilter* createColorLevelFilter();
CGEVignetteFilter* createVignetteFilter();
CGEVignetteExtFilter* createVignetteExtFilter();
CGETiltshiftVectorFilter* createTiltshiftVectorFilter();
CGETiltshiftEllipseFilter* createTiltshiftEllipseFilter();
CGETiltshiftVectorWithFixedBlurRadiusFilter* createFixedTiltshiftVectorFilter();
CGETiltshiftEllipseWithFixedBlurRadiusFilter* createFixedTiltshiftEllipseFilter();
CGESharpenBlurFastWithFixedBlurRadiusFilter* createSharpenBlurFastWithFixedBlurRadiusFilter();
CGEZoomBlurFilter* createZoomBlurFilter();
CGEZoomBlur2Filter* createZoomBlur2Filter();
CGEMotionBlurFilter* createMotionBlurFilter();
CGESelectiveColorFilter* createSelectiveColorFilter();
CGEExposureFilter* createExposureFilter();
CGEHueAdjustFilter* createHueAdjustFilter();
CGEColorBalanceFilter* createColorBalanceFilter();
CGELookupFilter* createLookupFilter();
}  // namespace CGE
#endif  // _CGE_FILTERS_H_

#endif
