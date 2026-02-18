/*
 * cge.h
 *
 * Main header file for CGE (Computer Graphics Extension) Library
 * Include this file to access all public APIs
 */

#ifndef _CGE_H_
#define _CGE_H_

// Core functionality
#include "../../src/core/cgeCommonDefine.h"
#include "../../src/core/cgeImageFilter.h"
#include "../../src/core/cgeImageHandler.h"
#include "../../src/core/cgeGLFunctions.h"
#include "../../src/core/cgeShaderFunctions.h"
#include "../../src/core/cgeTextureUtils.h"
#include "../../src/core/cgeGeometryUtils.h"
#include "../../src/core/cgeMat.h"
#include "../../src/core/cgeVec.h"
#include "../../src/core/cgeGlobal.h"

// Animation system
#include "../../src/animation/cgeAction.h"
#include "../../src/animation/cgeAnimation.h"

// Sprite system
#include "../../src/sprite/cgeSprite2d.h"
#include "../../src/sprite/cgeSprite3d.h"
#include "../../src/sprite/cgeSpriteCommon.h"
#include "../../src/sprite/cgeScene.h"


// Filter factory functions - use these to create filters
#include "filters.h"

// Platform abstraction
#include "platform.h"

#endif // _CGE_H_
