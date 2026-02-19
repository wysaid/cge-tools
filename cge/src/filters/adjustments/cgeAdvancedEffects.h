/*
 * cgeAdvancedEffects.h
 *
 *  Created on: 2013-12-13
 *      Author: Wang Yang
 */

#ifndef _CGEADVANCEDEFFECTS_H_
#define _CGEADVANCEDEFFECTS_H_

#include "../effects/cgeEmbossFilter.h"
#include "../effects/cgeEdgeFilter.h"
#include "../blur/cgeRandomBlurFilter.h"
#include "../blur/cgeBilateralBlurFilter.h"
#include "../blur/cgeMosaicBlurFilter.h"
#include "../beauty/cgeEnlargeEyeFilter.h"
#include "../beauty/cgeEyeTintFilter.h"
#include "../beauty/cgeEyeBrightenFilter.h"
#include "../beauty/cgeTeethWhitenFilter.h"
#include "../beauty/cgeMarscaraFilter.h"
#include "../beauty/cgeWrinkleRemoverFilter.h"
#include "../beauty/cgeRedeyeRemoveFilter.h"
#include "cgeLiquidationFilter.h"
#include "../effects/cgeLightFilter.h"
#include "../effects/cgeHalftoneFilter.h"
#include "../effects/cgePolarPixellateFilter.h"
#include "cgePolkaDotFilter.h"
#include "../effects/cgeCrosshatchFilter.h"
#include "../effects/cgeHazeFilter.h"
#include "../blur/cgeLerpblurFilter.h"

#include "../effects/cgeSketchFilter.h"

#include "../beauty/cgeBeautifyFilter.h"
#include "../effects/cgeAlienLookFilter.h"

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
