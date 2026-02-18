/*
* cgeCommonDefine.cpp
*
*  Created on: 2013-12-6
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#include "cgeCommonDefine.h"
#include <cassert>

#define CGE_VERSION "3.0.0"

extern "C"
{
    
    bool _cgeCheckGLError(const char* name, const char* file, int line)
    {
        int loopCnt = 0;
        for (GLenum error = glGetError(); loopCnt < 32 && error; error = glGetError(), ++loopCnt)
        {
            CGE_LOG_CODE(
                const char* pMsg;
                switch (error)
                {
                    case GL_INVALID_ENUM: pMsg = "invalid enum"; break;
                    case GL_INVALID_VALUE: pMsg = "invalid value"; break;
                    case GL_INVALID_OPERATION: pMsg = "invalid operation"; break;
                    case GL_OUT_OF_MEMORY: pMsg = "out of memory"; break;
                    case GL_INVALID_FRAMEBUFFER_OPERATION: pMsg = "invalid framebuffer operation"; break;
                    default: pMsg = "unknown error";
                }
            );
            CGE_LOG_ERROR("After \"%s\" glGetError %s(0x%x) at %s:%d\n", name, pMsg, error, file, line);
        }
        
        return loopCnt != 0;
    }
    
    const char* cgeGetVersion()
    {
        return CGE_VERSION;
    }
    
    void cgePrintGLString(const char* name, GLenum em)
    {
        CGE_LOG_INFO("GL_INFO %s = %s\n", name, glGetString(em));
    }
    
    const char* cgeGetBlendModeName(CGETextureBlendMode mode, bool withChinese)
    {
        if(mode < 0 || mode >= CGE_BLEND_TYPE_MAX_NUM)
            return nullptr;
        
        static CGEConstString s_blendModeName[] =
        {
            "mix", // 0
            "dissolve", // 1
            
            "darken", // 2
            "multiply", // 3
            "colorburn", // 4
            "linearburn", // 5
            "darkercolor", // 6
            
            "lighten", // 7
            "screen", // 8
            "colordodge", // 9
            "lineardodge", // 10
            "lightercolor", // 11
            
            "overlay", // 12
            "softlight", // 13
            "hardlight", // 14
            "vividlight", // 15
            "linearlight", // 16
            "pinlight", // 17
            "hardmix", // 18
            
            "difference", // 19
            "exclude", // 20
            "subtract", // 21
            "divide", // 22
            
            "hue", // 23
            "saturation", // 24
            "color", // 25
            "luminosity", // 26
            
            /////////////    More blend mode below (You can't see them in Adobe Photoshop)    //////////////
            
            "add",			// 19
            "addrev",	// 21
            "colorbw",		// 22
            
            /////////////    More blend mode above     //////////////
        };
        
        static CGEConstString s_blendModeNameWithChinese[] =
        {
            "mix(Normal)", // 0
            "dissolve(Dissolve)", // 1
            
            "darken(Darken)", // 2
            "multiply(Multiply)", // 3
            "color burn(Color Burn)", // 4
            "linear burn(Linear Burn)", // 5
            "darker color(Darker Color)", // 6
            
            "lighten(Lighten)", // 7
            "screen(Screen)", // 8
            "color dodge(Color Dodge)", // 9
            "linear dodge(Linear Dodge)", // 10
            "lighter color(Lighter Color)", // 11
            
            "overlay(Overlay)", // 12
            "soft light(Soft Light)", // 13
            "hard light(Hard Light)", // 14
            "vivid light(Vivid Light)", // 15
            "linear light(Linear Light)", // 16
            "pin light(Pin Light)", // 17
            "hard mix(Hard Mix)", // 18
            
            "difference(Difference)", // 19
            "exclude(Exclude)", // 20
            "subtract(Subtract)", // 21
            "divide(Divide)", // 22
            
            "hue(Hue)", // 23
            "saturation(Saturation)", // 24
            "color(Color)", // 25
            "luminosity(Luminosity)", // 26
            
            /////////////    More blend mode below (You can't see them in Adobe Photoshop)    //////////////
            
            "add(Add)",			// 19
            "addrev(Add Reverse)",	// 21
            "colorbw(Black & White)",		// 22
            
            /////////////    More blend mode above     //////////////
        };
        
        return withChinese ? s_blendModeNameWithChinese[mode] : s_blendModeName[mode];
    }
    
    void cgeSetGlobalBlendMode(const CGEGlobalBlendMode mode)
    {
#if CGE_TEXTURE_PREMULTIPLIED
        
        const GLenum BLEND_SRC = GL_ONE;
        
#else
        
        const GLenum BLEND_SRC = GL_SRC_ALPHA;
        
#endif
        
        switch (mode)
        {
            case CGEGLOBAL_BLEND_ALPHA:
                glBlendFunc(BLEND_SRC, GL_ONE_MINUS_SRC_ALPHA);
                break;
            case CGEGLOBAL_BLEND_ALPHA_SEPERATE:
                glBlendFuncSeparate(BLEND_SRC, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);
                break;
            case CGEGLOBAL_BLEND_ADD:
                glBlendFunc(BLEND_SRC, GL_ONE);
                break;
            case CGEGLOBAL_BLEND_ADD_SEPARATE:
                glBlendFuncSeparate(BLEND_SRC, GL_ONE, GL_ONE_MINUS_DST_ALPHA, GL_ONE);
                break;
            case CGEGLOBAL_BLEND_ADD_SEPARATE_EXT:
                glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE_MINUS_DST_ALPHA, GL_ONE);
                break;
            case CGEGLOBAL_BLEND_MULTIPLY:
                glBlendFunc(GL_ZERO, GL_SRC_COLOR);
                break;
            case CGEGLOBAL_BLEND_MULTIPLY_SEPERATE:
                glBlendFuncSeparate(BLEND_SRC, GL_SRC_COLOR, GL_ONE_MINUS_DST_ALPHA, GL_ONE);
                break;
            case CGEGLOBAL_BLEND_SCREEN:
                glBlendFunc(BLEND_SRC, GL_ONE_MINUS_SRC_COLOR);
                break;
            case CGEGLOBAL_BLEND_SCREEN_EXT:
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
                break;
            case CGEGLOBAL_BLEND_NONE:
                //fall through
            default:
                glDisable(GL_BLEND);
                return ;
        }
        
        glEnable(GL_BLEND);
    }
    
    void cgeGetDataAndChannelByFormat(CGEBufferFormat fmt, GLenum* dataFmt, GLenum* channelFmt, GLint* channel)
    {
        GLenum df, cf;
        GLint c;
        switch(fmt)
        {
            case CGE_FORMAT_RGB_INT8:
                df = GL_UNSIGNED_BYTE;
                cf = GL_RGB;
                c = 3;
                break;
            case CGE_FORMAT_RGBA_INT8:
                df = GL_UNSIGNED_BYTE;
                cf = GL_RGBA;
                c = 4;
                break;
            case CGE_FORMAT_RGB_INT16:
                df = GL_UNSIGNED_SHORT;
                cf = GL_RGB;
                c = 3;
                break;
            case CGE_FORMAT_RGBA_INT16:
                df = GL_UNSIGNED_SHORT;
                cf = GL_RGBA;
                c = 4;
                break;
            case CGE_FORMAT_RGB_FLOAT32:
                df = GL_FLOAT;
                cf = GL_RGB;
                c = 3;
                break;
            case CGE_FORMAT_RGBA_FLOAT32:
                df = GL_FLOAT;
                cf = GL_RGB;
                c = 4;
                break;
                
#ifdef GL_BGR
            case CGE_FORMAT_BGR_INT8:
                df = GL_UNSIGNED_BYTE;
                cf = GL_BGR;
                c = 3;
                break;
            case CGE_FORMAT_BGR_INT16:
                df = GL_UNSIGNED_SHORT;
                cf = GL_BGR;
                c = 3;
                break;
            case CGE_FORMAT_BGR_FLOAT32:
                df = GL_FLOAT;
                cf = GL_BGR;
                c = 3;
                break;
#endif
#ifdef GL_BGRA
            case CGE_FORMAT_BGRA_INT8:
                df = GL_UNSIGNED_BYTE;
                cf = GL_BGRA;
                c = 4;
                break;
            case CGE_FORMAT_BGRA_INT16:
                df = GL_UNSIGNED_SHORT;
                cf = GL_BGRA;
                c = 4;
                break;
            case CGE_FORMAT_BGRA_FLOAT32:
                df = GL_FLOAT;
                cf = GL_BGRA;
                c = 4;
                break;
#endif
#ifdef GL_RED_EXT
            case CGE_FORMAT_LUMINANCE:
                df = GL_UNSIGNED_BYTE;
                cf = GL_LUMINANCE;
                c = 1;
#endif
#ifdef GL_LUMINANCE_ALPHA
            case CGE_FORMAT_LUMINANCE_ALPHA:
                df = GL_UNSIGNED_BYTE;
                cf = GL_LUMINANCE_ALPHA;
                c = 2;
#endif
            default:
                df = GL_FALSE;
                cf = GL_FALSE;
                c = 0;
        }
        
        if(dataFmt != nullptr) *dataFmt = df;
        if(channelFmt != nullptr) *channelFmt = cf;
        if(channel != nullptr) *channel = c;
    }
    
    GLuint cgeGenTextureWithBuffer(const void* bufferData, GLint w, GLint h, GLenum channelFmt, GLenum dataFmt, GLint channel, GLint bindID, GLenum texFilter, GLenum texWrap)
    {
        assert(w != 0 && h != 0);
        GLuint tex;
        static const GLenum eArrs[] = { GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA};
        if(channel <= 0 || channel > 4)
            return 0;
        const GLenum& internalFormat = eArrs[channel - 1];
        glActiveTexture(GL_TEXTURE0 + bindID);
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, channelFmt, dataFmt, bufferData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texWrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texWrap);
        return tex;
    }
    
}