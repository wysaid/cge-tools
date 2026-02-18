/*
* cgeFaceBlendFilter.cpp
*
*  Created on: 2015-12-17
*      Author: Wang Yang
* Description: Face blending filter
*/

#include <vector>
#include "cgeFaceBlendFilter.h"
#include "../../core/cgeMat.h"

#define EYE_CIRCLE_PNTS 20

static CGEConstString s_vshFaceBlend = CGE_SHADER_STRING
(
attribute vec2 vPosition; // Target face position
attribute vec2 srcPosition; // Original face position
attribute float posDis; // Progressive blend weight

varying vec2 srcTexCoord;
varying vec2 dstTexCoord;
varying float srcWeight;

void main()
{
    gl_Position = vec4(vPosition * 2.0 - 1.0, 0.0, 1.0);
    srcTexCoord = srcPosition;
    dstTexCoord = vPosition;
    srcWeight = posDis;
}
);

static CGEConstString s_fshFaceBlend = CGE_SHADER_STRING_PRECISION_M
(
 varying vec2 srcTexCoord;
 varying vec2 dstTexCoord;
 varying float srcWeight;
 
 uniform sampler2D srcTex;
 uniform sampler2D dstTex;
 uniform float srcAvg;
 uniform float dstAvg;
 
 vec3 RGB2HSL(vec3 src)
{
    float maxc = max(max(src.r, src.g), src.b);
    float minc = min(min(src.r, src.g), src.b);
    float L = (maxc + minc) / 2.0;
    if(maxc == minc)
        return vec3(0.0, 0.0, L);
    float H, S;

    // Note: on some low-precision devices, N - (A+B) != N - A - B
    float temp1 = maxc - minc;
    S = mix(temp1 / (2.0 - maxc - minc), temp1 / (maxc + minc), step(L, 0.5));

    vec3 comp;
    comp.xy = vec2(equal(src.xy, vec2(maxc)));
    float comp_neg = 1.0 - comp.x;
    comp.y *= comp_neg;
    comp.z = (1.0 - comp.y) * comp_neg;

    float dif = maxc - minc;
    vec3 result = comp * vec3((src.g - src.b) / dif,
                              2.0 + (src.b - src.r) / dif,
                              4.0 + (src.r - src.g) / dif);
    
    H = result.x + result.y + result.z;

    H *= 60.0;
    H += step(H, 0.0) * 360.0;
    return vec3(H / 360.0, S, L); // H(0~1), S(0~1), L(0~1)
}
 
 vec3 HSL2RGB(vec3 src) // H, S, L
{
    float q = (src.z < 0.5) ? src.z * (1.0 + src.y) : (src.z + src.y - (src.y * src.z));
    float p = 2.0 * src.z - q;
    
    vec3 dst = vec3(src.x + 0.333, src.x, src.x - 0.333);
    
    
    dst = fract(dst);
    
    //////// Optimize ////////////////////////
    
    vec3 weight = step(dst, vec3(1.0 / 6.0));
    vec3 weight_neg = 1.0 - weight;
    
    vec3 weight2 = weight_neg * step(dst, vec3(0.5));
    vec3 weight2_neg = weight_neg * (1.0 - weight2);
    
    vec3 weight3 = weight2_neg * step(dst, vec3(2.0 / 3.0));
    vec3 weight4 = (1.0 - weight3) * weight2_neg;
    
    float q_p = q - p;
    
    dst = mix(dst, p + q_p * 6.0 * dst, weight);
    dst = mix(dst, vec3(q), weight2);
    dst = mix(dst, p + q_p * ((2.0 / 3.0) - dst) * 6.0, weight3);
    dst = mix(dst, vec3(p), weight4);
    
    return dst;
}
 
 \n
 %s\n
 
 void main()
{
    vec4 src = texture2D(srcTex, srcTexCoord);
    vec4 dst = texture2D(dstTex, dstTexCoord);
    
    vec3 srcHSL = RGB2HSL(src.rgb);
    vec3 dstHSL = RGB2HSL(dst.rgb);
    
    srcHSL.xy = dstHSL.xy;
    srcHSL.z = max(srcHSL.z + dstAvg - srcAvg, 0.0);
    
    src.rgb = HSL2RGB(srcHSL);
    
    src.a = smoothstep(0.0, 0.5, srcWeight);
    src.rgb = blend(dst.rgb, src.rgb, 1.0) * src.a;

    gl_FragColor = src;
}
);

namespace CGE
{
    CGEFaceBlendFilter::CGEFaceBlendFilter() : m_srcFaceVertexVBO(0), m_dstFaceVertexVBO(0), m_faceGradientVBO(0), m_faceMeshIndexVBO(0), m_cacheTexture(0), m_drawer(nullptr)
    {
        m_srcFaceFeature.texID = 0;
        m_dstFaceFeature.texID = 0;
    }

    CGEFaceBlendFilter::~CGEFaceBlendFilter()
    {
        _clearBuffers();
        delete m_drawer;
    }

    void CGEFaceBlendFilter::_clearBuffers()
    {
        CGE_DELETE_GL_OBJS(glDeleteBuffers, m_srcFaceVertexVBO, m_dstFaceVertexVBO, m_faceGradientVBO, m_faceMeshIndexVBO);
        CGE_DELETE_GL_OBJS(glDeleteTextures, m_srcFaceFeature.texID, m_dstFaceFeature.texID, m_cacheTexture);
    }
    
    bool CGEFaceBlendFilter::initWithBlendMode(CGETextureBlendMode blendMode)
    {
        const char* blendFuncStr = CGEBlendInterface::getShaderFuncByBlendMode(blendMode);
        if(blendFuncStr == nullptr)
            return false;

        char buffer[4096];

        sprintf(buffer, s_fshFaceBlend, blendFuncStr);

        glBindAttribLocation(m_program.programID(), 1, "srcPosition");
        glBindAttribLocation(m_program.programID(), 2, "posDis");
        if(initShadersFromString(s_vshFaceBlend, buffer))
        {
            m_program.bind();
            m_program.sendUniformi("srcTex", 0);
            m_program.sendUniformi("dstTex", 1);
            if(m_drawer == nullptr)
                m_drawer = TextureDrawer::create();
            return m_drawer != nullptr;
        }
        return false;
    }

    void CGEFaceBlendFilter::setFaces(const FaceFeature& srcFace, const FaceFeature& dstFace)
    {
        m_srcFaceFeature = srcFace;
        m_dstFaceFeature = dstFace;
        _genFaceBuffers();

        float srcEyeDis = (srcFace.leftEyePos - srcFace.rightEyePos).length();
        const Vec4f srcFaceArea = Vec4f(srcFace.leftEyePos[0], srcFace.leftEyePos[1], srcEyeDis, srcEyeDis);

        m_srcAvgColor = getAverageLumByTexture(m_framebuffer, m_srcFaceFeature.texID, srcFaceArea, srcFace.texSize);

        float dstEyeDis = (dstFace.leftEyePos - dstFace.rightEyePos).length();
        const Vec4f dstFaceArea = Vec4f(dstFace.leftEyePos[0], dstFace.leftEyePos[1], dstEyeDis, dstEyeDis);

        m_dstAvgColor = getAverageLumByTexture(m_framebuffer, m_dstFaceFeature.texID, dstFaceArea, dstFace.texSize);

        m_program.bind();
        m_program.sendUniformf("srcAvg", m_srcAvgColor);
        m_program.sendUniformf("dstAvg", m_dstAvgColor);
    }

    void CGEFaceBlendFilter::_genFaceBuffers()
    {
        GLuint buffers[4] = {0};
        glGenBuffers(4, buffers);

        m_srcFaceVertexVBO = buffers[0];
        m_dstFaceVertexVBO = buffers[1];
        m_faceGradientVBO = buffers[2];
        m_faceMeshIndexVBO = buffers[3];

        _assginBufferWithFaceFeature(m_srcFaceFeature, m_srcFaceVertexVBO);
        _assginBufferWithFaceFeature(m_dstFaceFeature, m_dstFaceVertexVBO);

        //mesh indices
        {
            std::vector<unsigned short> meshIndices;
            meshIndices.reserve(1000); // Enough points

            for(int i = 1; i != EYE_CIRCLE_PNTS; ++i)
            {
                meshIndices.push_back(0);
                meshIndices.push_back(i);
                meshIndices.push_back(i + 1);
            }

            const int rightEyeStart = EYE_CIRCLE_PNTS + 1;

            for(int i = 1; i != EYE_CIRCLE_PNTS; ++i)
            {
                meshIndices.push_back(rightEyeStart);
                meshIndices.push_back(i + rightEyeStart);
                meshIndices.push_back(i + (rightEyeStart + 1));
            }
            
            //////////////////////////////////////////////////////////////////////////
            
            unsigned short arr[] = {
                
                // Left eye gradient
                43, 42, 0,

                // Right eye gradient
                43, 21, 42,
                
                // Left eye
                0, 44, 43,
                // Right eye
                21, 43, 45,
                
                46, 43, 44,
                46, 44, 47,
                46, 47, 59,
                46, 59, 49,
                46, 49, 50,
                46, 50, 48,
                46, 48, 45,
                46, 45, 43,
            };
            
            for(auto i : arr)
            {
                meshIndices.push_back(i);
            }
            
            const int mouthChinStart = 49;
            
            for(int i = 1; i != 10; ++i)
            {
                meshIndices.push_back(mouthChinStart);
                meshIndices.push_back(mouthChinStart + i);
                meshIndices.push_back(mouthChinStart + i + 1);
            }

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_faceMeshIndexVBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndices.size() * sizeof(meshIndices[0]), meshIndices.data(), GL_STATIC_DRAW);

            m_drawCounts = (int)meshIndices.size();
        }

        //face gradients
        {
            std::vector<float> gradients;
            gradients.reserve(1000);

			//left eye
            gradients.push_back(1.0f);
            for(int i = 0; i != EYE_CIRCLE_PNTS; ++i)
            {
                gradients.push_back(0.0f);
            }

			//right eye
            gradients.push_back(1.0f);
            for(int i = 0; i != EYE_CIRCLE_PNTS; ++i)
            {
                gradients.push_back(0.0f);
            }

			//nose, mouth, chin

            gradients.push_back(0.0f); //42
            
            gradients.push_back(1.0f); //43
            gradients.push_back(0.0f); //44
            gradients.push_back(0.0f); //45
            gradients.push_back(1.0f); //46
            gradients.push_back(0.0f); //47
            gradients.push_back(0.0f); //48
            gradients.push_back(1.0f); //49
            
            //50 - 59
            for(int i = 0; i != 10; ++i)
                gradients.push_back(0.0f);

            glBindBuffer(GL_ARRAY_BUFFER, m_faceGradientVBO);
            glBufferData(GL_ARRAY_BUFFER, gradients.size() * sizeof(gradients[0]), gradients.data(), GL_STATIC_DRAW);
        }

    }

    void CGEFaceBlendFilter::_assginBufferWithFaceFeature(const FaceFeature& feature, GLuint vbo)
    {

        const Vec2f& leftEye = feature.leftEyePos;
        const Vec2f& rightEye = feature.rightEyePos;
        const Vec2f& midPos = (leftEye + rightEye) / 2.0f;
        const Vec2f& mouthPos = feature.mouthPos;
        const Vec2f& chinPos = feature.chinPos;
        float eyeDis = (leftEye - rightEye).length();
        float halfEyeDis = eyeDis * 0.6f;

        Vec2f&& rightDir = rightEye - leftEye;
        rightDir.normalize();
        
        const Vec2f& upDir = Mat2::makeRotation(M_PI * 0.5f) * rightDir;
        const Vec2f& upperMidPos = midPos - upDir * sqrtf(0.11f) * eyeDis;

        std::vector<Vec2f> vertices;
        vertices.reserve(200);

        vertices.push_back(leftEye); // Left eye

		const int maxPntNum = EYE_CIRCLE_PNTS - 1;

        //sqrt(0.11) * eyeDis

        const float totalRad = M_PI * 1.5f - acosf(5.0f / 6.0f);

        for(int i = 0; i != EYE_CIRCLE_PNTS; ++i)
        {
            const Vec2f& v = leftEye + Mat2::makeRotation(-i * totalRad / maxPntNum - 0.58569f) * rightDir * halfEyeDis;
            vertices.push_back(v);
        }

        vertices.push_back(rightEye); // Right eye

        for(int i = 0; i != EYE_CIRCLE_PNTS; ++i)
        {
            const Vec2f& v = rightEye - Mat2::makeRotation(i * totalRad / maxPntNum + 0.58569f) * rightDir * halfEyeDis;
            vertices.push_back(v);
        }

        // Force data correction to avoid diagonal artifacts
        vertices[1] = upperMidPos;
        vertices[22] = upperMidPos;

		const Vec2f& leftEyeDown = leftEye + upDir * halfEyeDis;
		const Vec2f& rightEyeDown = rightEye + upDir * halfEyeDis;

		const Vec2f& nosePos = CGE_MIX(midPos, mouthPos, 0.8);

        const float noseDis = (midPos - nosePos).length();
        const float mouthDis = (midPos - mouthPos).length();
        
        const bool customNosePos = (noseDis > halfEyeDis);
        const bool customMouthPos = (mouthDis > halfEyeDis);

		//bool isEyeDownEnough = leftEyeDown.y

		const Vec2f& noseLeft = customNosePos ? nosePos - rightDir * halfEyeDis * 1.0 : leftEyeDown;
		const Vec2f& noseRight = customNosePos ? nosePos + rightDir * halfEyeDis * 1.0 : rightEyeDown;

        const float mouthWide = halfEyeDis * 1.3f;
        const float chinDis = (mouthPos - chinPos).length();
        
		Vec2f&& mouthLeft = mouthPos - rightDir * mouthWide;  //50
		Vec2f&& mouthRight = mouthPos + rightDir * mouthWide; //51
        
        if(!customMouthPos)
        {
            mouthLeft[1] = leftEyeDown[1];
            mouthRight[1] = rightEyeDown[1];
            CGE_LOG_INFO("❕Warn: The mouths are too closed to the eyes...");
        }

        vertices.push_back(upperMidPos);    // 42 (newly added)

		vertices.push_back(midPos);			//43
		vertices.push_back(leftEyeDown);	//44
		vertices.push_back(rightEyeDown);	//45
		vertices.push_back(nosePos);		//46
		vertices.push_back(noseLeft);		//47
		vertices.push_back(noseRight);		//48
		vertices.push_back(mouthPos);		//49
        
        //50 - 59
        for(int i = 0; i != 10; ++i)
        {
            const Vec2f& v = mouthPos + Mat2::makeRotation(i * (M_PI / 9.0)) * rightDir * Vec2f(mouthWide, chinDis);
            vertices.push_back(v);
        }

        for(auto& elem : vertices)
        {
            elem /= feature.texSize;
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
    }
    
    double CGEFaceBlendFilter::getAverageLumByTexture(const FrameBuffer& framebuffer, GLuint texture, const Vec4f& area, const Vec2f& texSize)
    {
        static const int bufferLen = 64;

        if(m_cacheTexture == 0)
        {
            m_cacheTexture = cgeGenTextureWithBuffer(nullptr, bufferLen, bufferLen, GL_RGBA, GL_UNSIGNED_BYTE);
        }

        framebuffer.bindTexture2D(m_cacheTexture);

        const Vec2f& texScale = Vec2f(area[2], area[3]) / (float)bufferLen;
        const Vec2f& scaledSize = texSize / texScale;
        const Vec2f& pointDis = Vec2f(-area[0], -area[1]);
        const Vec2f& scaledPointDis = pointDis / texScale;

        glViewport(scaledPointDis[0], scaledPointDis[1], scaledSize[0], scaledSize[1]);

        m_drawer->drawTexture(texture);

        glFinish();

        unsigned char buffer[bufferLen * bufferLen * 4];

        glReadPixels(0, 0, bufferLen, bufferLen, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        
        long avg = 0;
        
        for(int i = 0; i != bufferLen * bufferLen; ++i)
        {
            int index = i * 4;
            long lum = ((long)buffer[index] * 9797) + ((long)buffer[index + 1] * 19234) + ((long)buffer[index + 2] * 3736);
            avg += lum >> 15;
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        return avg / (bufferLen * bufferLen * 255.0);
    }
    
    void CGEFaceBlendFilter::render2Texture(CGEImageHandlerInterface* handler, GLuint srcTexture, GLuint vertexBufferID)
    {
        CGE_LOG_CODE
        (
         if(m_drawer == nullptr)
         {
             CGE_LOG_ERROR("CGEFaceBlendFilter: drawer is null!!\n");
             assert(0);
             return;
         }
         )

        handler->setAsTarget();

        m_drawer->drawTexture(m_dstFaceFeature.texID);

        glFlush();

        m_program.bind();

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        glBindBuffer(GL_ARRAY_BUFFER, m_dstFaceVertexVBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, m_srcFaceVertexVBO);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, m_faceGradientVBO);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_srcFaceFeature.texID);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, srcTexture);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_faceMeshIndexVBO);
        glDrawElements(GL_TRIANGLES, m_drawCounts, GL_UNSIGNED_SHORT, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

        glDisable(GL_BLEND);

        cgeCheckGLError("glDrawArrays");
    }

}