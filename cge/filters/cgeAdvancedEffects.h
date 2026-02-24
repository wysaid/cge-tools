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

// Factory functions matching the public API in cgeFilters.h use
// CGEImageFilterInterface* to ensure consistent mangling on MSVC.
CGEImageFilterInterface* createEmbossFilter();
CGEImageFilterInterface* createEdgeFilter();
CGEImageFilterInterface* createRandomBlurFilter();
CGEImageFilterInterface* createBilateralBlurFilter();
CGEImageFilterInterface* createMosaicBlurFilter();
CGEImageFilterInterface* createEnlargeEyeFilter();
CGEImageFilterInterface* createEyeTintFilter();
CGEImageFilterInterface* createEyeBrightenFilter();
CGEImageFilterInterface* createTeethWhitenFilter();
CGEImageFilterInterface* createMarscaraFilter();
CGEImageFilterInterface* createRedeyeRemoveFilter();
CGEImageFilterInterface* createLightFilter();
CGEImageFilterInterface* createHalftoneFilter();
CGEImageFilterInterface* createPolarPixellateFilter();
CGEImageFilterInterface* createCrosshatchFilter();
CGEImageFilterInterface* createHazeFilter();
CGEImageFilterInterface* createSketchFilter();
CGEImageFilterInterface* createBeautifyFilter();
CGEImageFilterInterface* createAlienLookFilter();
}  // namespace CGE

#endif
