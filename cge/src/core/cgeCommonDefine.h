/*
 * cgeCommonDefine.h
 *
 *  Created on: 2013-12-6
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef _CGECOMMONDEFINE_H_
#define _CGECOMMONDEFINE_H_

#include "cgeGlobal.h"

#ifndef cgeCheckGLError
#ifdef CGE_LOG_ERROR
#define cgeCheckGLError(name) _cgeCheckGLError(name, __FILE__, __LINE__);
#else
#define cgeCheckGLError(name)
#endif
#endif

#ifndef _CGE_GET_MACRO_STRING_HELP
#define _CGE_GET_MACRO_STRING_HELP(x) #x
#endif
#ifndef CGE_GET_MACRO_STRING
#define CGE_GET_MACRO_STRING(x) _CGE_GET_MACRO_STRING_HELP(x)
#endif
#define CGE_FLOATCOMP0(x)	(x < 0.001f && x > -0.001f)

#define CGE_UNIFORM_MAX_LEN 32

#define CGE_DELETE(p) do { delete p; p = NULL; } while(0)
#define CGE_DELETE_ARR(p) do { delete[] p; p = NULL; } while(0)

/*
 To conserve texture resources, the following constraints apply to all OpenGL textures:
 Texture units 0 and 1 may be used multiple times during various initializations. If you need to bind a fixed texture,
 start from TEXTURE_START when using texture units.
 The required offset may increase, so
 use the macro below for addition instead of hardcoding GL_TEXTURE*
 
 */

#define CGE_TEXTURE_INPUT_IMAGE_INDEX 0
#define CGE_TEXTURE_INPUT_IMAGE GL_TEXTURE0

#define CGE_TEXTURE_OUTPUT_IMAGE_INDEX 1
#define CGE_TEXTURE_OUTPUT_IMAGE GL_TEXTURE1

#define CGE_TEXTURE_START_INDEX 2
#define CGE_TEXTURE_START GL_TEXTURE2

//Mark if the texture is premultiplied with the alpha channel.
#ifndef CGE_TEXTURE_PREMULTIPLIED
#define CGE_TEXTURE_PREMULTIPLIED 0
#endif

#ifdef _CGE_SHADER_VERSION_

#define CGE_GLES_ATTACH_STRING_L "#version " CGE_GET_MACRO_STRING(_CGE_SHADER_VERSION_) "\n#ifdef GL_ES\nprecision lowp float;\n#endif\n"
#define CGE_GLES_ATTACH_STRING_M "#version " CGE_GET_MACRO_STRING(_CGE_SHADER_VERSION_) "\n#ifdef GL_ES\nprecision mediump float;\n#endif\n"
#define CGE_GLES_ATTACH_STRING_H "#version " CGE_GET_MACRO_STRING(_CGE_SHADER_VERSION_) "\n#ifdef GL_ES\nprecision highp float;\n#endif\n"

#else

#define CGE_GLES_ATTACH_STRING_L "#ifdef GL_ES\nprecision lowp float;\n#endif\n"
#define CGE_GLES_ATTACH_STRING_M "#ifdef GL_ES\nprecision mediump float;\n#endif\n"
#define CGE_GLES_ATTACH_STRING_H "#ifdef GL_ES\nprecision highp float;\n#endif\n"
#endif

//Do not add any precision conf within SHADER_STRING_PRECISION_* macro!
#if defined(_MSC_VER) && _MSC_VER < 1600
//If the m$ compiler is under 10.0, there mustn't be any ',' outside the "()" in the shader string !!
//For exmaple: vec2(0.0, 0.0) --> YES.
//             float a, b;  --> No!!!, you must do like this: float a; float b;
#define CGE_SHADER_STRING_PRECISION_L(string) CGE_GLES_ATTACH_STRING_L  #string
#define CGE_SHADER_STRING_PRECISION_M(string) CGE_GLES_ATTACH_STRING_M  #string
#define CGE_SHADER_STRING_PRECISION_H(string) CGE_GLES_ATTACH_STRING_H  #string
#ifndef CGE_SHADER_STRING
#define CGE_SHADER_STRING(string) #string
#endif
#else
#define CGE_SHADER_STRING_PRECISION_L(...) CGE_GLES_ATTACH_STRING_L  #__VA_ARGS__
#define CGE_SHADER_STRING_PRECISION_M(...) CGE_GLES_ATTACH_STRING_M  #__VA_ARGS__
#define CGE_SHADER_STRING_PRECISION_H(...) CGE_GLES_ATTACH_STRING_H  #__VA_ARGS__
#ifndef CGE_SHADER_STRING
#define CGE_SHADER_STRING(...) #__VA_ARGS__
#endif
#endif

#define CGE_COMMON_CREATE_FUNC(cls, funcName) CGE_COMMON_CREATE_FUNC_WITH_NAME(cls, funcName, create)

#define CGE_COMMON_CREATE_FUNC_WITH_NAME(cls, funcName, creatorName) \
static inline cls* creatorName() \
{\
cls* instance = new cls(); \
if(!instance->funcName()) \
{ \
delete instance; \
instance = nullptr; \
CGE_LOG_ERROR("create %s failed!", #cls); \
} \
return instance; \
}

#define CGE_ARRAY_LEN(x) (sizeof(x) / sizeof(*x))

#ifdef __cplusplus

template <class T, int Len>
static inline int cgeArrLen(const T (&v)[Len])
{
    return Len;
}

#if defined(_MSC_VER) && _MSC_VER <= 1700

struct _cgeResetValue
{
	template <class T> static inline void r(T& t) { t = T(); }
	template <class T> static inline void r(T& t1, T& t2) { t1 = T(); t2 = T(); }
	template <class T> static inline void r(T& t1, T& t2, T& t3) { t1 = T(); t2 = T(); t3 = T(); }
	template <class T> static inline void r(T& t1, T& t2, T& t3, T& t4) { t1 = T(); t2 = T(); t3 = T(); t4 = T(); }
	template <class T> static inline void r(T& t1, T& t2, T& t3, T& t4, T& t5) { t1 = T(); t2 = T(); t3 = T(); t4 = T(); t5 = T(); }
	template <class T> static inline void r(T& t1, T& t2, T& t3, T& t4, T& t5, T& t6) { t1 = T(); t2 = T(); t3 = T(); t4 = T(); t5 = T(); t6 = T(); }
	template <class T> static inline void r(T& t1, T& t2, T& t3, T& t4, T& t5, T& t6, T& t7) { t1 = T(); t2 = T(); t3 = T(); t4 = T(); t5 = T(); t6 = T(); t7 = T(); }
	template <class T> static inline void r(T& t1, T& t2, T& t3, T& t4, T& t5, T& t6, T& t7, T& t8) { t1 = T(); t2 = T(); t3 = T(); t4 = T(); t5 = T(); t6 = T(); t7 = T(); t8 = T(); }
};

#define cgeResetValue(...) _cgeResetValue::r(__VA_ARGS__)

#else

template <typename T>
static inline void cgeResetValue(T& t)
{
    t = T();
}

template <typename T, typename ...ARGS>
static inline void cgeResetValue(T& t, ARGS&... args)
{
    t = T();
    cgeResetValue(args...);
}

#endif

#define CGE_DELETE_GL_OBJS(func, ...) \
do\
{\
    GLuint objs[] = {__VA_ARGS__}; \
    func(cgeArrLen(objs), objs); \
    cgeResetValue(__VA_ARGS__); \
}while(0)

template<class T>
class CGEBlockLimit
{
    CGEBlockLimit& operator=(const CGEBlockLimit& other) { return *this; }
public:
    explicit CGEBlockLimit(const T& _func) : func(_func) {}
    ~CGEBlockLimit() { func(); }
    
private:
    const T& func;
};

template<class T>
inline CGEBlockLimit<const T&> ___cgeMakeBlockLimit(const T& f)
{
    return CGEBlockLimit<const T&>(f);
}

#define __cgeMakeBlockLimit(ARG, ANYSIGN) \
const auto& ANYSIGN = ___cgeMakeBlockLimit(ARG); \
(void)ANYSIGN;  // Avoid warning for unused variable.

#define _cgeMakeBlockLimit(ARG, VAR, LINE) __cgeMakeBlockLimit(ARG, VAR ## LINE)
#define _cgeMakeBlockLimit_(ARG, VAR, LINE) _cgeMakeBlockLimit(ARG, VAR, LINE)
#define cgeMakeBlockLimit(...) _cgeMakeBlockLimit_(__VA_ARGS__, _blockVar, __LINE__)

namespace CGE
{
#ifndef CGE_MIN
    
    template<typename Type>
    inline Type CGE_MIN(Type a, Type b)
    {
        return a < b ? a : b;
    }
    
#endif
    
#ifndef CGE_MAX
    
    template<typename Type>
    inline Type CGE_MAX(Type a, Type b)
    {
        return a > b ? a : b;
    }
    
#endif
    
#ifndef CGE_MID
    
    template<typename Type>
    inline Type CGE_MID(Type n, Type vMin, Type vMax)
    {
        if(n < vMin)
            n = vMin;
        else if(n > vMax)
            n = vMax;
        return n;
    }
    
#endif
    
#ifndef CGE_MIX
    
    template<typename OpType, typename MixType>
    inline auto CGE_MIX(OpType a, OpType b, MixType value) -> decltype(a - a * value + b * value)
    {
        return a - a * value + b * value;
    }
    
#endif
}


extern "C" {
#endif
    
    typedef const char* const CGEConstString;
    
    typedef enum CGEBufferFormat
    {
        CGE_FORMAT_RGB_INT8,
        CGE_FORMAT_RGB_INT16,
        CGE_FORMAT_RGB_FLOAT32,
        CGE_FORMAT_RGBA_INT8,
        CGE_FORMAT_RGBA_INT16,
        CGE_FORMAT_RGBA_FLOAT32,
#ifdef GL_BGR
        CGE_FORMAT_BGR_INT8,
        CGE_FORMAT_BGR_INT16,
        CGE_FORMAT_BGR_FLOAT32,
#endif
#ifdef GL_BGRA
        CGE_FORMAT_BGRA_INT8,
        CGE_FORMAT_BGRA_INT16,
        CGE_FORMAT_BGRA_FLOAT32,
#endif
#ifdef GL_LUMINANCE
        CGE_FORMAT_LUMINANCE, // 8 bit
#endif
#ifdef GL_LUMINANCE_ALPHA
        CGE_FORMAT_LUMINANCE_ALPHA, //8+8 bit
#endif
        
    }CGEBufferFormat;
    
    typedef enum CGETextureBlendMode
    {
        CGE_BLEND_MIX,            // 0 Normal
        CGE_BLEND_DISSOLVE,       // 1 Dissolve
        
        CGE_BLEND_DARKEN,         // 2 Darken
        CGE_BLEND_MULTIPLY,       // 3 Multiply
        CGE_BLEND_COLORBURN,      // 4 Color Burn
        CGE_BLEND_LINEARBURN,     // 5 Linear Burn
        CGE_BLEND_DARKER_COLOR,   // 6 Darker Color
        
        CGE_BLEND_LIGHTEN,        // 7 Lighten
        CGE_BLEND_SCREEN,         // 8 Screen
        CGE_BLEND_COLORDODGE,     // 9 Color Dodge
        CGE_BLEND_LINEARDODGE,    // 10 Linear Dodge
        CGE_BLEND_LIGHTERCOLOR,  // 11 Lighter Color
        
        CGE_BLEND_OVERLAY,        // 12 Overlay
        CGE_BLEND_SOFTLIGHT,      // 13 Soft Light
        CGE_BLEND_HARDLIGHT,      // 14 Hard Light
        CGE_BLEND_VIVIDLIGHT,     // 15 Vivid Light
        CGE_BLEND_LINEARLIGHT,    // 16 Linear Light
        CGE_BLEND_PINLIGHT,       // 17 Pin Light
        CGE_BLEND_HARDMIX,        // 18 Hard Mix
        
        CGE_BLEND_DIFFERENCE,     // 19 Difference
        CGE_BLEND_EXCLUDE,        // 20 Exclude
        CGE_BLEND_SUBTRACT,       // 21 Subtract
        CGE_BLEND_DIVIDE,         // 22 Divide
        
        CGE_BLEND_HUE,            // 23 Hue
        CGE_BLEND_SATURATION,     // 24 Saturation
        CGE_BLEND_COLOR,          // 25 Color
        CGE_BLEND_LUMINOSITY,     // 26 Luminosity
        
        /////////////    More blend mode below (You can't see them in Adobe Photoshop)    //////////////
        
        CGE_BLEND_ADD,			  // 27
        CGE_BLEND_ADDREV,         // 28
        CGE_BLEND_COLORBW,		  // 29
        
        /////////////    More blend mode above     //////////////
        
        CGE_BLEND_TYPE_MAX_NUM //Its value defines the max num of blend.
    }CGETextureBlendMode;
    
    typedef enum CGEGlobalBlendMode
    {
        CGEGLOBAL_BLEND_NONE,
        CGEGLOBAL_BLEND_ALPHA,
        CGEGLOBAL_BLEND_ALPHA_SEPERATE,
        CGEGLOBAL_BLEND_ADD,
        CGEGLOBAL_BLEND_ADD_SEPARATE,
        CGEGLOBAL_BLEND_ADD_SEPARATE_EXT, // The EXT variant ignores alpha premultiplication
        CGEGLOBAL_BLEND_MULTIPLY,
        CGEGLOBAL_BLEND_MULTIPLY_SEPERATE,
        CGEGLOBAL_BLEND_SCREEN,
        CGEGLOBAL_BLEND_SCREEN_EXT,
    }CGEGlobalBlendMode;
    
    const char* cgeGetVersion();
    void cgePrintGLString(const char*, GLenum);
    bool _cgeCheckGLError(const char* name, const char* file, int line); // Use cgeCheckGLError directly instead
    
    ////////////////////////////////////

    void cgeSetGlobalBlendMode(const CGEGlobalBlendMode mode);
    void cgeGetDataAndChannelByFormat(CGEBufferFormat fmt, GLenum* dataFmt, GLenum* channelFmt, GLint* channel);
    
#ifdef __cplusplus
    const char* cgeGetBlendModeName(const CGETextureBlendMode mode, bool withChinese = false);
    GLuint cgeGenTextureWithBuffer(const void* bufferData, GLint w, GLint h, GLenum channelFmt, GLenum dataFmt, GLint channels = 4, GLint bindID = 0, GLenum texFilter = GL_LINEAR, GLenum texWrap = GL_CLAMP_TO_EDGE);
#else
    const char* cgeGetBlendModeName(const CGETextureBlendMode mode, bool withChinese);
    GLuint cgeGenTextureWithBuffer(const void* bufferData, GLint w, GLint h, GLenum channelFmt, GLenum dataFmt, GLint channels, GLint bindID , GLenum texFilter, GLenum texWrap);
#endif
    
#ifdef __cplusplus
}
#endif

#endif /* _CGECOMMONDEFINE_H_ */
