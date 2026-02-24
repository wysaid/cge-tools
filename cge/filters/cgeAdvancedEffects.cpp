/*
 * cgeAdvancedEffects.cpp
 *
 *  Created on: 2013-12-13
 *      Author: Wang Yang
 */

// Include cgeFilters.h first so _CGE_FILTERS_H_ is defined, ensuring
// factory function declarations use the public API return type
// (CGEImageFilterInterface*). This prevents MSVC linker errors caused
// by return-type name mangling mismatches.
#include "../cgeFilters.h"

#include "cgeAdvancedEffects.h"

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
CGEImageFilterInterface* createEmbossFilter()
{
    COMMON_FUNC(CGEEmbossFilter);
}

CGEImageFilterInterface* createEdgeFilter()
{
    COMMON_FUNC(CGEEdgeFilter);
}

CGEEdgeSobelFilter* createEdgeSobelFilter()
{
    COMMON_FUNC(CGEEdgeSobelFilter);
}

CGEImageFilterInterface* createRandomBlurFilter()
{
    COMMON_FUNC(CGERandomBlurFilter);
}

CGEImageFilterInterface* createBilateralBlurFilter()
{
    COMMON_FUNC(CGEBilateralBlurFilter);
}

CGEBilateralBlurBetterFilter* createBilateralBlurBetterFilter()
{
    COMMON_FUNC(CGEBilateralBlurBetterFilter);
}

CGEImageFilterInterface* createMosaicBlurFilter()
{
    COMMON_FUNC(CGEMosaicBlurFilter);
}

CGEImageFilterInterface* createEnlargeEyeFilter()
{
    COMMON_FUNC(CGEEnlargeEyeFilter);
}

CGEImageFilterInterface* createEyeTintFilter()
{
    COMMON_FUNC(CGEEyeTintFilter);
}

CGEImageFilterInterface* createEyeBrightenFilter()
{
    COMMON_FUNC(CGEEyeBrightenFilter);
}

CGEImageFilterInterface* createTeethWhitenFilter()
{
    COMMON_FUNC(CGETeethWhitenFilter);
}

CGEImageFilterInterface* createMarscaraFilter()
{
    COMMON_FUNC(CGEMarscaraFilter);
}

CGEWrinkleRemoveFilter* createWrinkleRemoveFilter()
{
    COMMON_FUNC(CGEWrinkleRemoveFilter);
}

CGEImageFilterInterface* createRedeyeRemoveFilter()
{
    COMMON_FUNC(CGERedeyeRemoveFilter);
}

CGEImageFilterInterface* createLightFilter()
{
    COMMON_FUNC(CGELightFilter);
}

CGELiquidationFilter* getLiquidationFilter(float ratio, float stride)
{
    CGELiquidationFilter* proc = new CGELiquidationFilter;
    if (!proc->initWithMesh(ratio, stride))
    {
        delete proc;
        return nullptr;
    }
    return proc;
}

CGELiquidationFilter* getLiquidationFilter(float width, float height, float stride)
{
    CGELiquidationFilter* proc = new CGELiquidationFilter;
    if (!proc->initWithMesh(width, height, stride))
    {
        delete proc;
        return nullptr;
    }
    return proc;
}

CGELiquidationNicerFilter* getLiquidationNicerFilter(float ratio, float stride)
{
    CGELiquidationNicerFilter* proc = new CGELiquidationNicerFilter;
    if (!proc->initWithMesh(ratio, stride))
    {
        delete proc;
        return nullptr;
    }
    return proc;
}

CGELiquidationNicerFilter* getLiquidationNicerFilter(float width, float height, float stride)
{
    CGELiquidationNicerFilter* proc = new CGELiquidationNicerFilter;
    if (!proc->initWithMesh(width, height, stride))
    {
        delete proc;
        return nullptr;
    }
    return proc;
}

CGEImageFilterInterface* createHalftoneFilter()
{
    COMMON_FUNC(CGEHalftoneFilter);
}

CGEImageFilterInterface* createPolarPixellateFilter()
{
    COMMON_FUNC(CGEPolarPixellateFilter);
}

CGEPolkaDotFilter* createPolkaDotFilter()
{
    COMMON_FUNC(CGEPolkaDotFilter);
}

CGEImageFilterInterface* createCrosshatchFilter()
{
    COMMON_FUNC(CGECrosshatchFilter);
}

CGEImageFilterInterface* createHazeFilter()
{
    COMMON_FUNC(CGEHazeFilter);
}

CGELerpblurFilter* createLerpblurFilter()
{
    COMMON_FUNC(CGELerpblurFilter);
}

CGEImageFilterInterface* createSketchFilter()
{
    COMMON_FUNC(CGESketchFilter);
}

CGEImageFilterInterface* createBeautifyFilter()
{
    COMMON_FUNC(CGEBeautifyFilter);
}

CGEImageFilterInterface* createAlienLookFilter()
{
    COMMON_FUNC(CGEAlienLookFilter);
}

}  // namespace CGE
