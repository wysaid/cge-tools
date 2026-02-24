/*
 * cgeFilterBasic.cpp
 *
 *  Created on: 2013-12-25
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

// Include cgeFilters.h first so _CGE_FILTERS_H_ is defined, ensuring
// factory function declarations use the public API return type
// (CGEImageFilterInterface*). This prevents MSVC linker errors caused
// by return-type name mangling mismatches.
#include "../cgeFilters.h"

#include "cgeFilterBasic.h"

#define COMMON_FUNC(type)    \
    type* proc = new type(); \
    if (!proc->init())       \
    {                        \
        delete proc;         \
        proc = NULL;         \
    }                        \
    return proc;

namespace CGE
{
CGEImageFilterInterface* createBrightnessFilter()
{
    COMMON_FUNC(CGEBrightnessFilter);
}

CGEImageFilterInterface* createBrightnessFastFilter()
{
    COMMON_FUNC(CGEBrightnessFastFilter);
}

CGEImageFilterInterface* createContrastFilter()
{
    COMMON_FUNC(CGEContrastFilter);
}

CGEImageFilterInterface* createSharpenBlurFilter()
{
    COMMON_FUNC(CGESharpenBlurFilter);
}

CGEImageFilterInterface* createSharpenBlurFastFilter()
{
    COMMON_FUNC(CGESharpenBlurFastFilter);
}

CGEImageFilterInterface* createSharpenBlurSimpleFilter()
{
    COMMON_FUNC(CGESharpenBlurSimpleFilter);
}

CGEImageFilterInterface* createSharpenBlurSimpleBetterFilter()
{
    COMMON_FUNC(CGESharpenBlurSimpleBetterFilter);
}

CGEImageFilterInterface* createSaturationHSLFilter()
{
    COMMON_FUNC(CGESaturationHSLFilter);
}

CGEImageFilterInterface* createSaturationFilter()
{
    COMMON_FUNC(CGESaturationFilter);
}

CGEImageFilterInterface* createShadowHighlightFilter()
{
    COMMON_FUNC(CGEShadowHighlightFilter);
}

CGEImageFilterInterface* createShadowHighlightFastFilter()
{
    COMMON_FUNC(CGEShadowHighlightFastFilter);
}

CGEImageFilterInterface* createWhiteBalanceFilter()
{
    COMMON_FUNC(CGEWhiteBalanceFilter);
}

CGEImageFilterInterface* createWhiteBalanceFastFilter()
{
    COMMON_FUNC(CGEWhiteBalanceFastFilter);
}

CGEImageFilterInterface* createMonochromeFilter()
{
    COMMON_FUNC(CGEMonochromeFilter);
}

CGEImageFilterInterface* createCurveTexFilter()
{
    COMMON_FUNC(CGECurveTexFilter);
}

CGEImageFilterInterface* createCurveFilter()
{
    COMMON_FUNC(CGECurveFilter);
}

CGEImageFilterInterface* createMoreCurveFilter()
{
    COMMON_FUNC(CGEMoreCurveFilter);
}

CGEImageFilterInterface* createMoreCurveTexFilter()
{
    COMMON_FUNC(CGEMoreCurveTexFilter);
}

CGEImageFilterInterface* createColorLevelFilter()
{
    COMMON_FUNC(CGEColorLevelFilter);
}

CGEImageFilterInterface* createVignetteFilter()
{
    COMMON_FUNC(CGEVignetteFilter);
}

CGEImageFilterInterface* createVignetteExtFilter()
{
    COMMON_FUNC(CGEVignetteExtFilter);
}

CGEImageFilterInterface* createTiltshiftVectorFilter()
{
    COMMON_FUNC(CGETiltshiftVectorFilter);
}

CGEImageFilterInterface* createTiltshiftEllipseFilter()
{
    COMMON_FUNC(CGETiltshiftEllipseFilter);
}

CGEImageFilterInterface* createFixedTiltshiftVectorFilter()
{
    COMMON_FUNC(CGETiltshiftVectorWithFixedBlurRadiusFilter);
}

CGEImageFilterInterface* createFixedTiltshiftEllipseFilter()
{
    COMMON_FUNC(CGETiltshiftEllipseWithFixedBlurRadiusFilter);
}

CGEImageFilterInterface* createSharpenBlurFastWithFixedBlurRadiusFilter()
{
    COMMON_FUNC(CGESharpenBlurFastWithFixedBlurRadiusFilter);
}

CGEImageFilterInterface* createZoomBlurFilter()
{
    COMMON_FUNC(CGEZoomBlurFilter);
}

CGEImageFilterInterface* createZoomBlur2Filter()
{
    COMMON_FUNC(CGEZoomBlur2Filter);
}

CGEImageFilterInterface* createMotionBlurFilter()
{
    COMMON_FUNC(CGEMotionBlurFilter);
}

CGEImageFilterInterface* createSelectiveColorFilter()
{
    COMMON_FUNC(CGESelectiveColorFilter);
}

CGEImageFilterInterface* createExposureFilter()
{
    COMMON_FUNC(CGEExposureFilter);
}

CGEImageFilterInterface* createHueAdjustFilter()
{
    COMMON_FUNC(CGEHueAdjustFilter);
}

CGEImageFilterInterface* createColorBalanceFilter()
{
    COMMON_FUNC(CGEColorBalanceFilter);
}

CGEImageFilterInterface* createLookupFilter()
{
    COMMON_FUNC(CGELookupFilter);
}
}  // namespace CGE