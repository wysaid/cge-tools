/*
 * filters.h
 *
 * Filter factory functions - public API for creating filters
 * All filters should be created through these factory functions
 */

#ifndef _CGE_FILTERS_H_
#define _CGE_FILTERS_H_

#include "cgeImageFilter.h"

namespace CGE
{
// ============ Adjustments (Basic) ============
class CGEImageFilterInterface;

// Brightness
CGEImageFilterInterface* createBrightnessFilter();
CGEImageFilterInterface* createBrightnessFastFilter();

// Contrast
CGEImageFilterInterface* createContrastFilter();

// Saturation
CGEImageFilterInterface* createSaturationFilter();
CGEImageFilterInterface* createSaturationHSLFilter();

// Colors
CGEImageFilterInterface* createHueAdjustFilter();
CGEImageFilterInterface* createExposureFilter();
CGEImageFilterInterface* createWhiteBalanceFilter();
CGEImageFilterInterface* createWhiteBalanceFastFilter();
CGEImageFilterInterface* createColorBalanceFilter();
CGEImageFilterInterface* createColorLevelFilter();
CGEImageFilterInterface* createMonochromeFilter();
CGEImageFilterInterface* createSelectiveColorFilter();

// Curves
CGEImageFilterInterface* createCurveFilter();
CGEImageFilterInterface* createCurveTexFilter();
CGEImageFilterInterface* createMoreCurveFilter();
CGEImageFilterInterface* createMoreCurveTexFilter();

// Shadow/Highlight
CGEImageFilterInterface* createShadowHighlightFilter();
CGEImageFilterInterface* createShadowHighlightFastFilter();

// Vignette
CGEImageFilterInterface* createVignetteFilter();
CGEImageFilterInterface* createVignetteExtFilter();

// ============ Blur Filters ============
CGEImageFilterInterface* createGaussianBlurFilter();
CGEImageFilterInterface* createBilateralBlurFilter();
CGEImageFilterInterface* createMotionBlurFilter();
CGEImageFilterInterface* createZoomBlurFilter();
CGEImageFilterInterface* createZoomBlur2Filter();
CGEImageFilterInterface* createMosaicBlurFilter();
CGEImageFilterInterface* createLerpBlurFilter();
CGEImageFilterInterface* createRandomBlurFilter();

// Tiltshift
CGEImageFilterInterface* createTiltshiftVectorFilter();
CGEImageFilterInterface* createTiltshiftEllipseFilter();
CGEImageFilterInterface* createFixedTiltshiftVectorFilter();
CGEImageFilterInterface* createFixedTiltshiftEllipseFilter();
CGEImageFilterInterface* createFastTiltShiftFilter();

// ============ Sharpen ============
CGEImageFilterInterface* createSharpenBlurFilter();
CGEImageFilterInterface* createSharpenBlurFastFilter();
CGEImageFilterInterface* createSharpenBlurSimpleFilter();
CGEImageFilterInterface* createSharpenBlurSimpleBetterFilter();
CGEImageFilterInterface* createSharpenBlurFastWithFixedBlurRadiusFilter();

// ============ Beauty Filters ============
CGEImageFilterInterface* createBeautifyFilter();
CGEImageFilterInterface* createEnlargeEyeFilter();
CGEImageFilterInterface* createEyeBrightenFilter();
CGEImageFilterInterface* createEyeTintFilter();
CGEImageFilterInterface* createTeethWhitenFilter();
CGEImageFilterInterface* createWrinkleRemoverFilter();
CGEImageFilterInterface* createRedeyeRemoveFilter();
CGEImageFilterInterface* createMarscaraFilter();
CGEImageFilterInterface* createFaceBlendFilter();
CGEImageFilterInterface* createSkinSmoothFilter();

// ============ Effects Filters ============
CGEImageFilterInterface* createEmbossFilter();
CGEImageFilterInterface* createEdgeFilter();
CGEImageFilterInterface* createEdgeExtractFilter();
CGEImageFilterInterface* createSketchFilter();
CGEImageFilterInterface* createCrosshatchFilter();
CGEImageFilterInterface* createHalftoneFilter();
CGEImageFilterInterface* createPolkadotFilter();
CGEImageFilterInterface* createPolarPixellateFilter();
CGEImageFilterInterface* createPixellateFilter();
CGEImageFilterInterface* createPosterizeFilter();
CGEImageFilterInterface* createHazeFilter();
CGEImageFilterInterface* createLightFilter();
CGEImageFilterInterface* createAlienLookFilter();

// ============ Distortion ============
CGEImageFilterInterface* createDistortionFilter();
CGEImageFilterInterface* createLiquidationFilter();

// ============ Blend & Lookup ============
CGEImageFilterInterface* createBlendFilter();
CGEImageFilterInterface* createLookupFilter();
CGEImageFilterInterface* createColorMappingFilter();

// ============ Advanced Filters ============
CGEImageFilterInterface* createMultipleEffectsFilter();
CGEImageFilterInterface* createDynamicFiltersFilter();
CGEImageFilterInterface* createDynamicWaveFilter();
CGEImageFilterInterface* createAdvancedEffectsFilter();

// ============ MinMax Filters ============
CGEImageFilterInterface* createMinValueFilter();
CGEImageFilterInterface* createMaxValueFilter();
CGEImageFilterInterface* createMidValueFilter();
CGEImageFilterInterface* createHistogramFilter();

// ============ Border Effects ============
CGEImageFilterInterface* createBorderEffectsFilter();
CGEImageFilterInterface* createCardEffectsFilter();
}  // namespace CGE

#endif  // _CGE_FILTERS_H_
