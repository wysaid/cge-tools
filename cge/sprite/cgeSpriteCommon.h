/*
 * cgeSpriteCommon.h
 *
 *  Created on: 2014-9-25
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#if !defined(_CGESPRITECOMMON_H_) && !defined(_CGE_ONLY_FILTERS_)
#define _CGESPRITECOMMON_H_

#include "cgeMat.h"
#include "cgeGLFunctions.h"
#include "cgeShaderFunctions.h"

// This file contains some helper classes and functions needed by sprites

namespace CGE
{
void cgeSpritesInitBuiltin();
void cgeSpritesCleanupBuiltin();

class SpriteCommonSettings
{
public:
    SpriteCommonSettings();
    virtual ~SpriteCommonSettings();

    static CGESizei sCanvasSize;  // Canvas size for Sprite2d drawing (global; must be set in advance)

    inline static void sSetCanvasSize(int w, int h)
    {
        sCanvasSize.set(w, h);
        sOrthoProjectionMatrix = Mat4::makeOrtho(0.0f, (float)w, 0.0f, (float)h, -1e3f, 1e3f);
    }

    // Only applies global default settings to sprites created afterward. For sprites already created, use the set*Flip
    // functions individually
    static void sFlipCanvas(bool x, bool y);
    static void sFlipSprite(bool x, bool y);

    CGE_LOG_CODE(static std::vector<SpriteCommonSettings*>& getDebugManager();)

protected:
    static Mat4 sOrthoProjectionMatrix;
    static bool sCanvasFlipX, sCanvasFlipY;
    static bool sSpriteFlipX, sSpriteFlipY;
};

class GeometryLineStrip2d
{
public:
    GeometryLineStrip2d();
    ~GeometryLineStrip2d();

    inline void setLineWidth(float lineWidth) { m_lineWidth = lineWidth; }

    inline void setColor(float r, float g, float b, float a = 1.0f) { m_color = Vec4f(r, g, b, a); }

    inline void setGradient(float gradient) { m_gradient = gradient; }

    inline void clearPoints() { m_points.clear(); }

    inline void pushPoint(const Vec2f& pnt) { m_points.push_back(pnt); }
    inline void pushPoint(float x, float y)
    {
        Vec2f pnt(x, y);
        m_points.push_back(pnt);
    }
    inline void pushPoints(std::vector<Vec2f> v);
    void flush();

    void render();


    static float sFlipX, sFlipY;
    static inline void sSetFlip(bool fx, bool fy)
    {
        sFlipX = fx ? -1.0f : 1.0f;
        sFlipY = fy ? -1.0f : 1.0f;
    }

    static void sClearProgram();

    static void inline sSetLineFlip(float fx, float fy)
    {
        s_program->bind();
        s_program->sendUniformf(paramLineFlipName, fx, fy);
    }

protected:
    static CGEConstString paramAttribPositionName;
    static CGEConstString paramAttribLineDataName;
    static CGEConstString paramLineWidthName;
    static CGEConstString paramLineFlipName;
    static CGEConstString paramColorName;
    static CGEConstString paramGradientName;

    static GLint s_lineWidthLocation;
    static GLint s_lineFlipLocation;
    static GLint s_canvasSizeLocation;
    static GLint s_colorLocation;
    static GLint s_gradientLocation;

    static bool _initProgram();
    void _setUniforms();

protected:
    static ProgramObject* s_program;
    static GLuint s_posAttribLocation, s_lineAttribLocation;
    GLuint m_posBuffer, m_lineBuffer;  //, m_elementArrayBuffer;
    std::vector<Vec2f>::size_type m_posBufferLen, m_lineBufferLen;
    std::vector<Vec2f> m_points, m_vecPos;
    std::vector<Vec3f> m_vecLineData;
    Vec4f m_color;
    float m_lineWidth, m_gradient;
};

// Not currently used
// 	class GeometryLineStripWithTexture2d : public GeometryLineStrip2d
// 	{
// 	public:
// 		GeometryLineStripWithTexture2d() {}
// 		~GeometryLineStripWithTexture2d() {}
//
//
// 	protected:
// 		SharedTexture m_texture;
// 	};

// Special usage; see Sprite2dExt
class SharedPoint
{
public:
    SharedPoint() : m_pnt(nullptr), m_refCount(nullptr) {}
    SharedPoint(const SharedPoint& other) : m_pnt(nullptr), m_refCount(nullptr) { *this = other; }

    SharedPoint(const Vec2f& v)
    {
        m_refCount = new int(1);
        m_pnt = new Vec2f(v);
    }

    SharedPoint(float x, float y)
    {
        m_refCount = new int(1);
        m_pnt = new Vec2f(x, y);
    }

    ~SharedPoint();

    inline SharedPoint& operator=(const SharedPoint& other)
    {
        assert(this != &other);
        if (m_refCount != nullptr && --*m_refCount <= 0)
        {
            clear();
        }

        m_pnt = other.m_pnt;
        m_refCount = other.m_refCount;
        ++*m_refCount;
        return *this;
    }

    inline Vec2f& pos()
    {
        assert(m_pnt != nullptr);
        return *m_pnt;
    }
    inline void setPos(const Vec2f& v) { *m_pnt = v; }
    inline void setPos(float x, float y)
    {
        m_pnt->x() = x;
        m_pnt->y() = y;
    }

protected:
    void clear();

private:
    mutable Vec2f* m_pnt;
    mutable int* m_refCount;
};

}  // namespace CGE

#endif
