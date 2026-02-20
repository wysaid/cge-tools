/*
 * cgeSprite3d.h
 *
 *  Created on: 2014-10-16
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#if !defined(_CGESPRITE3d_H_) && !defined(_CGE_ONLY_FILTERS_)
#define _CGESPRITE3d_H_

#include "cgeSpriteCommon.h"

namespace CGE
{
// Provides only a unified interface consistent with SpriteInterface2d.
class SpriteInterface3d : public SpriteCommonSettings
{
public:
    SpriteInterface3d();
    virtual ~SpriteInterface3d() {}

    inline Vec3f& getPosition() { return m_pos; }
    inline Vec3f& getScaling() { return m_scaling; }
    inline Vec3f& getHotspot() { return m_hotspot; }
    inline Mat3& getRotation() { return m_rotation; }
    inline float getAlpha() const { return m_alpha; }
    inline float getZ() const { return m_zIndex; }

    virtual void setHotspot(const Vec3f& hotspot) { m_hotspot = hotspot; }

    virtual void setHotspot2Center() { m_hotspot = 0.0f; }

    virtual void move(const Vec3f& pos) { m_pos += pos; }

    virtual void moveX(float x) { m_pos[0] += x; }

    virtual void moveY(float y) { m_pos[1] += y; }

    virtual void moveZ(float z) { m_pos[2] += z; }

    virtual void moveTo(const Vec3f& pos) { m_pos = pos; }

    virtual void scale(const Vec3f& scaling) { m_scaling *= scaling; }

    virtual void scaleX(float sx) { m_scaling[0] *= sx; }

    virtual void scaleY(float sy) { m_scaling[1] *= sy; }

    virtual void scaleZ(float sz) { m_scaling[2] *= sz; }

    virtual void scaleTo(const Vec3f& scaling) { m_scaling = scaling; }

    virtual void rotate(float rad, float x, float y, float z) { m_rotation.rotate(rad, x, y, z); }

    virtual void rotateX(float rx) { m_rotation.rotateX(rx); }

    virtual void rotateY(float ry) { m_rotation.rotateY(ry); }

    virtual void rotateZ(float rz) { m_rotation.rotateZ(rz); }

    virtual void rotateTo(float rad, float x, float y, float z) { m_rotation = Mat3::makeRotation(rad, x, y, z); }

    virtual void rotateToX(float rx) { m_rotation = Mat3::makeXRotation(rx); }

    virtual void rotateToY(float ry) { m_rotation = Mat3::makeYRotation(ry); }

    virtual void rotateToZ(float rz) { m_rotation = Mat3::makeZRotation(rz); }

    virtual void restoreRotation() { m_rotation.loadIdentity(); }

    virtual void setAlpha(float alpha) { m_alpha = alpha; }

    virtual void setZ(float z) { m_zIndex = z; }

    virtual void render() {}

    inline bool operator<(const SpriteInterface3d& other) const { return m_zIndex < other.m_zIndex; }

    static bool compZ(const SpriteInterface3d& left, const SpriteInterface3d& right)
    {
        return left.m_zIndex < right.m_zIndex;
    }

    static bool compZp(const SpriteInterface3d* left, const SpriteInterface3d* right)
    {
        return left->m_zIndex < right->m_zIndex;
    }

protected:
    static CGEConstString paramAttribPositionName;
    static CGEConstString paramModelViewProjectionMatrixName;
    static CGEConstString paramRotationName;
    static CGEConstString paramScalingName;
    static CGEConstString paramTranslationName;
    static CGEConstString paramHotspotName;


protected:
    Vec3f m_pos;      // Sprite3d position
    Vec3f m_scaling;  // Sprite3d scaling
    Vec3f m_hotspot;  // Sprite3d pivot position, relative to model size; center (default) is (0,0,0)
    Mat3 m_rotation;  // Sprite3d rotation, representing rotations about the pivot in the (yOz), (xOz), (xOy) planes.
    float m_alpha;    // Sprite3d alpha (opacity)
    float m_zIndex;   // Sprite3d z-index (no actual significance; Sprite3d coordinates contain the z value; this zIndex
                     // is used only as a rendering sort key during animation)
};

//////////////////////////////////////////////////////////////////////////

typedef struct SpriteArrayBufferComponent
{
    SpriteArrayBufferComponent() :
        bufferData(nullptr),
        bufferID(0),
        componentSize(0),
        bufferBytes(0),
        elementCnt(0),
        bufferKind(GL_FALSE),
        bufferUsage(GL_FALSE),
        bufferDataKind(GL_FALSE),
        bufferStride(0)
    {}
    SpriteArrayBufferComponent(const void* buffer, GLsizei compSize, GLsizei bytes, GLenum bufKind, GLenum bufUsage,
                               GLenum dataKind, GLsizei elemCnt = 0, GLsizei stride = 0) :
        bufferData(buffer),
        bufferID(0),
        componentSize(compSize),
        bufferBytes(bytes),
        elementCnt(elemCnt),
        bufferKind(bufKind),
        bufferUsage(bufUsage),
        bufferDataKind(dataKind),
        bufferStride(stride)
    {}

    SpriteArrayBufferComponent(GLuint bufID, int compSize, int bytes, GLenum bufKind, GLenum bufUsage, GLenum dataKind,
                               GLsizei elemCnt = 0, GLsizei stride = 0) :
        bufferData(nullptr),
        bufferID(0),
        componentSize(compSize),
        bufferBytes(bytes),
        elementCnt(elemCnt),
        bufferKind(bufKind),
        bufferUsage(bufUsage),
        bufferDataKind(dataKind),
        bufferStride(stride)
    {}

    const void* bufferData;
    GLuint bufferID;
    GLsizei componentSize;  // Size of each component (not bytes)
    GLsizei bufferBytes;    // Buffer size
    GLsizei elementCnt;
    GLenum bufferKind, bufferUsage;  // bufferKind: GL_ARRAY_BUFFER/GL_ELEMENT_ARRAY_BUFFER, bufferUsage:
                                     // GL_STATIC_DRAW/GL_DYNAMIC_DRAW/GL_STREAM_DRAW
    GLenum bufferDataKind;  // Buffer data type GL_FLOAT/GL_UNSIGNED_SHORT
    GLsizei bufferStride;   // Buffer stride
} ArrayBufferComponent;

// For demonstration only; for more flexible use, inherit and override the shader and other parts
class Sprite3dExt : public virtual SpriteInterface3d
{
    // 	protected:
    // 		Sprite3d(bool bInitProgram) : SpriteInterface3d() { assert(!bInitProgram); }

public:
    Sprite3dExt() : SpriteInterface3d() {}
    ~Sprite3dExt();

    // Subclasses must override their own init method; do not use the init function polymorphically.
    bool init(const ArrayBufferComponent& vertex, const ArrayBufferComponent& vertexElementArray, GLenum drawFunc);

    virtual void renderWithMat(const Mat4& modelViewProjectionMatrix);

protected:
    bool _initProgram(CGEConstString vsh, CGEConstString fsh);
    virtual void _bindProgramLocations();
    virtual void _initProgramUniforms();
    inline GLuint genBufferWithComponent(const ArrayBufferComponent& component)
    {
        GLuint bufferID = 0;
        glGenBuffers(1, &bufferID);
        glBindBuffer(component.bufferKind, bufferID);
        glBufferData(component.bufferKind, component.bufferBytes, component.bufferData, component.bufferUsage);
        return bufferID;
    }

protected:
    GLuint m_vertAttribLocation;
    GLint m_mvpLocation;

protected:
    ProgramObject m_program;
    ArrayBufferComponent m_vertBuffer, m_vertElementBuffer;  // Contains geometry only
    GLenum m_drawFunc;
};


//////////////////////////////////////////////////////////////////////////

// Advanced usage; incompatible with Sprite3d
class Sprite3dAdvanced
{
public:
protected:
};

}  // namespace CGE

#endif
