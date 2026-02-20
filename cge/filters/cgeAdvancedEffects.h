/*
 * cgeAdvancedEffects.h
 *
 *  Created on: 2013-12-13
 *      Author: Wang Yang
 */

#ifndef _CGEADVANCEDEFFECTS_H_
#define _CGEADVANCEDEFFECTS_H_

#include "cgeEmbossFilter.h"
#include "cgeEdgeFilter.h"
#include "cgeRandomBlurFilter.h"
#include "cgeBilateralBlurFilter.h"
#include "cgeMosaicBlurFilter.h"
#include "cgeEnlargeEyeFilter.h"
#include "cgeEyeTintFilter.h"
#include "cgeEyeBrightenFilter.h"
#include "cgeTeethWhitenFilter.h"
#include "cgeMarscaraFilter.h"
#include "cgeWrinkleRemoverFilter.h"
#include "cgeRedeyeRemoveFilter.h"
#include "cgeLiquidationFilter.h"
#include "cgeLightFilter.h"
#include "cgeHalftoneFilter.h"
#include "cgePolarPixellateFilter.h"
#include "cgePolkaDotFilter.h"
#include "cgeCrosshatchFilter.h"
#include "cgeHazeFilter.h"
#include "cgeLerpblurFilter.h"

#include "cgeSketchFilter.h"

#include "cgeBeautifyFilter.h"
#include "cgeAlienLookFilter.h"

namespace CGE
{
// These functions have unique names not in filters.h - always declare them
CGEEdgeSobelFilter* createEdgeSobelFilter();
CGEBilateralBlurBetterFilter* createBilateralBlurBetterFilter();
CGEPolkaDotFilter* createPolkaDotFilter();
CGELerpblurFilter* createLerpblurFilter();
CGEWrinkleRemoveFilter* createWrinkleRemoveFilter();
CGELiquidationFilter* getLiquidationFilter(float ratio, float stride);
CGELiquidationFilter* getLiquidationFilter(float width, float height, float stride);
CGELiquidationNicerFilter* getLiquidationNicerFilter(float ratio, float stride);
CGELiquidationNicerFilter* getLiquidationNicerFilter(float width, float height, float stride);

// These functions have the same name in filters.h but different return types.
// Only declare with concrete types when filters.h has NOT been included.
#ifndef _CGE_FILTERS_H_
CGEEmbossFilter* createEmbossFilter();
CGEEdgeFilter* createEdgeFilter();
CGERandomBlurFilter* createRandomBlurFilter();
CGEBilateralBlurFilter* createBilateralBlurFilter();
CGEMosaicBlurFilter* createMosaicBlurFilter();
CGEEnlargeEyeFilter* createEnlargeEyeFilter();
CGEEyeTintFilter* createEyeTintFilter();
CGEEyeBrightenFilter* createEyeBrightenFilter();
CGETeethWhitenFilter* createTeethWhitenFilter();
CGEMarscaraFilter* createMarscaraFilter();
CGERedeyeRemoveFilter* createRedeyeRemoveFilter();
CGELightFilter* createLightFilter();
CGEHalftoneFilter* createHalftoneFilter();
CGEPolarPixellateFilter* createPolarPixellateFilter();
CGECrosshatchFilter* createCrosshatchFilter();
CGEHazeFilter* createHazeFilter();
CGESketchFilter* createSketchFilter();
CGEBeautifyFilter* createBeautifyFilter();
CGEAlienLookFilter* createAlienLookFilter();
#endif  // _CGE_FILTERS_H_
}  // namespace CGE

#endif
