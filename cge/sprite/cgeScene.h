/*
 * cgeScene.h
 *
 *  Created on: 2014-10-27
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef _CGE_SCENE_H_
#define _CGE_SCENE_H_

#include "cgeMat.h"
#include "cgeCommonDefine.h"

namespace CGE
{
// Helper class providing common scene navigation methods and parameters
class SceneInterfaceHelper
{
protected:
    SceneInterfaceHelper() :
        m_eye(0.0f, 0.0f),
        m_lookDir(0.0f, 1000.0f),
        m_eyeZ(0.0f),
        m_lookZ(0.0f),
        m_dirLen(1000.0f),
        m_fovyRad(M_PI / 4.0f),
        m_zNear(1.0f),
        m_zFar(1e5f),
        m_upDirRot(0.0f)
    {}

public:
    inline Mat4& modelViewMatrix() { return m_modelViewMatrix; }
    inline Mat4& projectionMatrix() { return m_projectionMatrix; }
    inline Vec2f& eyeXY() { return m_eye; }
    inline float& eyeZ() { return m_eyeZ; }
    inline const Vec2f& lookDirXY() const { return m_lookDir; }
    inline float& lookDirZ() { return m_lookZ; }
    inline float& fovyRad() { return m_fovyRad; }
    inline float& zNear() { return m_zNear; }
    inline float& zFar() { return m_zFar; }

    static inline float maxLookUp() { return M_PI / 2.4f; }

    inline void updatePerspective()
    {
        m_projectionMatrix = Mat4::makePerspective(m_fovyRad, m_aspect, m_zNear, m_zFar);
    }

    inline void setEye(const Vec3f& v)
    {
        m_eye[0] = v[0];
        m_eye[1] = v[1];
        m_eyeZ = v[2];
    }

    inline void lookIn(float rad)
    {
        m_fovyRad = CGE_MID<float>(m_fovyRad + rad, M_PI / 100.0f, M_PI / 2.0f);
        updatePerspective();
    }

    inline void lookInTo(float rad)
    {
        m_fovyRad = CGE_MID<float>(rad, M_PI / 100.0f, M_PI / 2.0f);
        updatePerspective();
    }

    inline void setViewport(float w, float h) { m_aspect = w / h; }
    inline void setAspect(float aspect) { m_aspect = aspect; }

    inline void rotateByLookDir(float r) { m_upDirRot = r; }

protected:
    Mat4 m_modelViewMatrix, m_projectionMatrix;
    Vec2f m_eye, m_lookDir;
    float m_eyeZ, m_lookZ;
    float m_dirLen;

    // Field of view angle of the frustum (radians), near clip plane, far clip plane
    float m_fovyRad, m_zNear, m_zFar;
    float m_aspect;
    float m_upDirRot;
};

// 	SceneInterface provides scene navigation helper interface (optional)
//
// 	Note: SceneInterface uses the xOy plane as the navigation ground by default, with the positive z-axis pointing
// upward 	SceneInterface only provides relevant matrices and vectors; it does not provide any rendering interface.
class SceneInterface : public SceneInterfaceHelper
{
public:
    SceneInterface(float w = 1.0f, float h = 1.0f) : SceneInterfaceHelper()
    {
        m_aspect = w / h;
        updatePerspective();
        updateView();
    }

    inline void updateView()
    {
        Vec2f center(m_eye + m_lookDir);
        float len = m_lookDir.length();
        float tmp = -m_lookZ / len;
        Vec2f dirBack(m_lookDir * tmp);
        Vec3f upDir(dirBack[0], dirBack[1], len);
        if (m_upDirRot !=
            0.0f)  // No need for exact float-to-zero comparison; this just checks whether it has been set to 0
            upDir = Mat3::makeRotation(m_upDirRot, m_lookDir[0], m_lookDir[1], m_lookZ) * upDir;
        m_modelViewMatrix = Mat4::makeLookAt(m_eye[0], m_eye[1], m_eyeZ, center[0], center[1], m_eyeZ + m_lookZ,
                                             upDir[0], upDir[1], upDir[2]);
    }

    inline void setEyeXY(const Vec2f& v) { m_eye = v; }

    inline void setEyeZ(float z) { m_eyeZ = z; }

    // Note: this sets the viewing direction and must not be zero
    inline void setLookdirXY(const Vec2f& v)
    {
        assert(v[0] || v[1]);
        m_lookDir = v * (m_dirLen / v.length());
    }

    inline void setLookdirZ(float z)
    {
        const float lookUpMax = m_dirLen * 3.732f;  // tan(PI / 75);
        m_lookZ = CGE_MID<float>(z, -lookUpMax, lookUpMax);
    }

    // Same as above; looking straight up is not allowed during scene navigation (tilting the head cannot form a flat
    // angle)
    inline void setLookdir(const Vec3f& v)
    {
        assert(v[0] || v[1]);
        m_lookDir[0] = v[0];
        m_lookDir[1] = v[1];
        m_lookDir.normalize();
        m_lookDir *= m_dirLen;
        const float lookUpMax = m_dirLen * 3.732f;  // tan(PI / 75);
        m_lookZ = CGE_MID<float>(v[2], -lookUpMax, lookUpMax);
    }

    // Rotate view; rad > 0 turns right
    inline void turn(float rad) { m_lookDir = Mat2::makeRotation(rad) * m_lookDir; }

    // Rotate view; rad > 0 is to the right.
    inline void turnTo(float rad) { m_lookDir = Vec2f(sinf(rad), cosf(rad)) * m_dirLen; }

    // Look upward; motion calculation relationship
    // Look-up angle formula: arctan(tan("current up angle") + motion) - "current up angle"
    inline void lookUp(float motion)
    {
        const float lookUpMax = m_dirLen * 3.732f;  // tan(PI / 75);
        m_lookZ = CGE_MID(m_lookZ + motion * m_dirLen, -lookUpMax, lookUpMax);
    }

    // Look up to (radians); directly look up to the specified angle
    // Range [-PI/2.4, PI/2.4], approximately ±75 degrees
    inline void lookUpTo(float rad) { m_lookZ = tanf(CGE_MID<float>(rad, -M_PI / 2.4f, M_PI / 2.4f)) * m_dirLen; }

    inline void goForward(float motion) { m_eye += m_lookDir * (motion / m_dirLen); }

    inline void goBack(float motion) { m_eye -= m_lookDir * (motion / m_dirLen); }

    inline void goLeft(float motion)
    {
        float m = motion / m_dirLen;
        m_eye[0] -= m_lookDir[1] * m;
        m_eye[1] += m_lookDir[0] * m;
    }

    inline void goRight(float motion)
    {
        float m = motion / m_dirLen;
        m_eye[0] += m_lookDir[1] * m;
        m_eye[1] -= m_lookDir[0] * m;
    }
};

class SceneInterfaceDescartes : public SceneInterfaceHelper
{
public:
    SceneInterfaceDescartes(float w = 1.0f, float h = 1.0f) : SceneInterfaceHelper()
    {
        m_aspect = w / h;
        updatePerspective();
        updateView();
    }

    inline void updateView()
    {
        Vec2f center(m_lookDir + Vec2f(m_eye[0], m_eyeZ));
        float len = m_lookDir.length();
        float tmp = -m_lookZ / len;
        Vec2f dirBack(m_lookDir * tmp);
        Vec3f upDir(dirBack[0], len, dirBack[1]);
        if (m_upDirRot !=
            0.0f)  // No need for exact float-to-zero comparison; this just checks whether it has been set to 0
            upDir = Mat3::makeRotation(m_upDirRot, m_lookDir[0], m_lookZ, m_lookDir[1]) * upDir;
        m_modelViewMatrix = Mat4::makeLookAt(m_eye[0], m_eye[1], m_eyeZ, center[0], m_eye[1] + m_lookZ, center[1],
                                             upDir[0], upDir[1], upDir[2]);
    }

    inline void setEyeXZ(const Vec2f& v)
    {
        m_eye[0] = v[0];
        m_eyeZ = v[1];
    }

    inline void setEyeY(float y) { m_eye[1] = y; }

    // Note: this sets the viewing direction and must not be zero
    inline void setLookdirXZ(const Vec2f& v)
    {
        assert(v[0] || v[1]);
        m_lookDir = v * (m_dirLen / v.length());
    }

    inline void setLookdirY(float z)
    {
        const float lookUpMax = m_dirLen * 3.732f;  // tan(PI / 75);
        m_lookZ = CGE_MID<float>(z, -lookUpMax, lookUpMax);
    }

    // Same as above; looking straight up is not allowed during scene navigation (tilting the head cannot form a flat
    // angle)
    inline void setLookdir(const Vec3f& v)
    {
        assert(v[0] || v[2]);
        m_lookDir[0] = v[0];
        m_lookDir[1] = v[2];
        m_lookDir.normalize();
        m_lookDir *= m_dirLen;
        const float lookUpMax = m_dirLen * 3.732f;  // tan(PI / 75);
        m_lookZ = CGE_MID<float>(v[1], -lookUpMax, lookUpMax);
    }

    // Rotate view; rad > 0 turns right
    inline void turn(float rad) { m_lookDir = Mat2::makeRotation(rad) * m_lookDir; }

    // Rotate view; rad > 0 is to the right.
    inline void turnTo(float rad) { m_lookDir = Vec2f(sinf(rad), cosf(rad)) * m_dirLen; }

    // Look upward; motion calculation relationship
    // Look-up angle formula: arctan(tan("current up angle") + motion) - "current up angle"
    inline void lookUp(float motion)
    {
        const float lookUpMax = m_dirLen * 3.732f;  // tan(PI / 75);
        m_lookZ = CGE_MID(m_lookZ + motion * m_dirLen, -lookUpMax, lookUpMax);
    }

    // Look up to (radians); directly look up to the specified angle
    // Range [-PI/2.4, PI/2.4], approximately ±75 degrees
    inline void lookUpTo(float rad) { m_lookZ = tanf(CGE_MID<float>(rad, -M_PI / 2.4f, M_PI / 2.4f)) * m_dirLen; }

    inline void goForward(float motion) { m_eye += m_lookDir * (motion / m_dirLen); }

    inline void goBack(float motion) { m_eye -= m_lookDir * (motion / m_dirLen); }

    inline void goLeft(float motion)
    {
        float m = motion / m_dirLen;
        m_eye[0] -= m_lookDir[1] * m;
        m_eye[1] += m_lookDir[0] * m;
    }

    inline void goRight(float motion)
    {
        float m = motion / m_dirLen;
        m_eye[0] += m_lookDir[1] * m;
        m_eye[1] -= m_lookDir[0] * m;
    }
};
}  // namespace CGE

#endif