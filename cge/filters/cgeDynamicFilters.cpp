/*
 * cgeDynamicFilters.cpp
 *
 *  Created on: 2015-11-18
 *      Author: Wang Yang
 */

// Include cgeFilters.h first so _CGE_FILTERS_H_ is defined, ensuring
// factory function declarations use the public API return type
// (CGEImageFilterInterface*). This prevents MSVC linker errors caused
// by return-type name mangling mismatches.
#include "../cgeFilters.h"

#include "cgeDynamicFilters.h"

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

CGEImageFilterInterface* createDynamicWaveFilter()
{
    COMMON_FUNC(CGEDynamicWaveFilter);
}
}  // namespace CGE