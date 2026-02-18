/*
* cgeSprite2d.h
*
*  Created on: 2014-9-9
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#if !defined(_CGESPRITE2D_H_) && !defined(_CGE_ONLY_FILTERS_)
#define _CGESPRITE2D_H_

#include "cgeSpriteCommon.h"

namespace CGE
{

	class GeometryLineStrip2d;
	class SharedPoint;

	class SpriteInterface2d : public SpriteCommonSettings
	{
	public:
		SpriteInterface2d();
		//Template compatibility interface, should not be called
// 		SpriteInterface2d(GLuint textureID, int width, int height) { assert(0); }
// 		//Template compatibility interface, should not be called
// 		SpriteInterface2d(const SharedTexture& texture) { assert(0); }
//		virtual ~SpriteInterface2d();

		inline const CGE::Vec2f& getPosition() const { return m_pos; }
		inline const CGE::Vec2f& getScaling() const { return m_scaling; }
		inline const CGE::Vec2f& getHotspot() const { return m_hotspot; }
		inline float getRotation() const { return m_rotation; }
		inline float getAlpha() const { return m_alpha; }
		inline float getZ() const { return m_zIndex; }

		//(0, 0) represents the center, (-1, -1) the top-left corner, (1, 1) the bottom-right corner
		virtual void setHotspot(float sx, float sy)
		{
			m_hotspot[0] = sx;
			m_hotspot[1] = sy;
		}

		virtual void setHotspot2Center()
		{
			m_hotspot[0] = 0;
			m_hotspot[1] = 0;
		}

		virtual void move(float dx, float dy)
		{
			m_pos[0] += dx;
			m_pos[1] += dy;
		}

		virtual void moveTo(float x, float y)
		{
			m_pos[0] = x;
			m_pos[1] = y;
		}

		virtual void scale(float dsx, float dsy)
		{
			m_scaling[0] *= dsx;
			m_scaling[1] *= dsy;
		}

		virtual void scaleTo(float sx, float sy)
		{
			m_scaling[0] = sx;
			m_scaling[1] = sy;
		}

		virtual void rotate(float dRot)
		{
			m_rotation += dRot;
		}

		virtual void rotateTo(float rot)
		{
			m_rotation = rot;
		}

		virtual void setAlpha(float alpha)
		{
			m_alpha = alpha;
		}

		virtual void setZ(float z)
		{
			m_zIndex = z;
		}

		virtual void render() {	}

		inline bool operator<(const SpriteInterface2d& other) const
		{
			return m_zIndex < other.m_zIndex;
		}

		static bool compZ(const SpriteInterface2d& left, const SpriteInterface2d& right)
		{
			return left.m_zIndex < right.m_zIndex;
		}

		static bool compZp(const SpriteInterface2d* left, const SpriteInterface2d* right)
		{
			return left->m_zIndex < right->m_zIndex;
		}

	protected:
		static CGEConstString paramAttribPositionName;
		static CGEConstString paramProjectionMatrixName;
		static CGEConstString paramHalfTexSizeName;
		static CGEConstString paramRotationName;
		static CGEConstString paramScalingName;
		static CGEConstString paramTranslationName;
		static CGEConstString paramHotspotName;
		static CGEConstString paramAlphaName;
		static CGEConstString paramZIndexName;
		static CGEConstString paramTextureName;
		static CGEConstString paramFilpCanvasName;
		static CGEConstString paramFlipSpriteName;
		static CGEConstString paramBlendColorName;

	protected:
		CGE::Vec2f m_pos; //Sprite2d position
		CGE::Vec2f m_scaling; //Sprite2d scaling
		CGE::Vec2f m_hotspot; //Sprite2d pivot position (relative to sprite size)
		float m_rotation; //Sprite2d rotation
		float m_alpha; //Sprite2d alpha (opacity)
		float m_zIndex; //Sprite2d z-value (reference only, not used for sorting)
	};

	//Use virtual inheritance to allow timeline extensibility
	class Sprite2d : public virtual SpriteInterface2d
	{
	protected:
		Sprite2d(bool bInitProgram);
		Sprite2d(GLuint textureID, int width, int height, bool bInitProgram);
		Sprite2d(const SharedTexture& texture, bool bInitProgram);

		Sprite2d(); //Compatibility interface, should not be called

	public:
		static ShaderObject *spVertexShader, *spFragmentShader;
		static Sprite2d* sCreateWithBuffer(const void* bytes, int width, int height, CGEBufferFormat format);
		static CGEConstString getVertexString();
		static CGEConstString getFragmentString();

		Sprite2d(GLuint textureID, int width, int height);
		Sprite2d(const SharedTexture& texture);
		virtual ~Sprite2d();

		SharedTexture& getTexture() { return m_texture; }
		void setTexture(const SharedTexture& tex);

        inline void setProjection(float* proj)
        {
            m_program.bind();
            glUniformMatrix4fv(m_projectionLocation, 1, false, proj);
        }
        
		virtual void setCanvasSize(int width, int height)
		{
			Mat4 m = Mat4::makeOrtho(0.0f, (float)width, 0.0f, (float)height, -1e6f, 1e6f);
            setProjection(m[0]);
		}

		virtual void restoreCanvasSize()
		{
			m_program.bind();
			glUniformMatrix4fv(m_projectionLocation, 1, false, sOrthoProjectionMatrix[0]);
		}

		//The Cartesian coordinate system has an inverted Y-axis compared to screen coordinates, so the default is vertically flipped.
		//To draw the sprite into a texture, disable the flip.
		virtual void setCanvasFlip(bool flipX, bool flipY)
		{
			float fx = flipX ? -1.0f : 1.0f;
			float fy = flipY ? -1.0f : 1.0f;
			m_program.bind();
			glUniform2f(m_canvasFlipLocation, fx, fy);
		}

		void restoreCanvasFlip()
		{
			float fx = sCanvasFlipX ? -1.0f : 1.0f;
			float fy = sCanvasFlipY ? -1.0f : 1.0f;
			m_program.bind();
			glUniform2f(m_canvasFlipLocation, fx, fy);
		}

		//Vertically flipped by default, so images appear correctly when drawn directly to the canvas.
		virtual void setSpriteFlip(bool flipX, bool flipY)
		{
			float fx = flipX ? -1.0f : 1.0f;
			float fy = flipY ? -1.0f : 1.0f;
			m_program.bind();
			glUniform2f(m_spriteFilpLocation, fx, fy);
		}

		void restoreSpriteFlip()
		{
			float fx = sSpriteFlipX ? -1.0f : 1.0f;
			float fy = sSpriteFlipY ? -1.0f : 1.0f;
			m_program.bind();
			glUniform2f(m_spriteFilpLocation, fx, fy);
		}

		virtual void setZ(float z)  //z value range: -1e20 ~ 1e20 (precision depends on float)
		{
			m_zIndex = z;
			m_program.bind();
			glUniform1f(m_zIndexLocation, z);
		}

		virtual void setAlpha(float alpha)
		{
			m_alpha = alpha;
			m_program.bind();
			glUniform1f(m_alphaLocation, alpha);
		}

		virtual void setHotspot(float sx, float sy)
		{
			m_hotspot[0] = sx;
			m_hotspot[1] = sy;
			m_program.bind();
			glUniform2f(m_hotspotLocation, sx, sy);
		}

		virtual void setHotspot2Center()
		{
			m_hotspot[0] = 0.0f;
			m_hotspot[1] = 0.0f;
			m_program.bind();
			glUniform2f(m_hotspotLocation, 0.0f, 0.0f);
		}

		virtual void move(float dx, float dy)
		{
			m_pos[0] += dx;
			m_pos[1] += dy;
		}

		virtual void moveTo(float x, float y)
		{
			m_pos[0] = x;
			m_pos[1] = y;
		}

		virtual void scale(float dsx, float dsy)
		{
			m_scaling[0] *= dsx;
			m_scaling[1] *= dsy;
		}

		virtual void scaleTo(float sx, float sy)
		{
			m_scaling[0] = sx;
			m_scaling[1] = sy;
		}

		virtual void rotate(float dRot)
		{
			m_rotation += dRot;			
		}

		virtual void rotateTo(float rot)
		{
			m_rotation = rot;
		}

		virtual void render();

	private:
		bool _initProgram();
		bool _tryLinkWithStaticShader();
		void _clearProgram();

	protected:
		//Abstracted draw method to give subclasses more flexible rendering control.
		virtual void _drawFunc();
		void _initProgramVars();

		GLint m_posAttribLocation;
		GLint m_projectionLocation;
		GLint m_halfTexLocation;
		GLint m_rotationLocation;
		GLint m_scalingLocation;
		GLint m_translationLocation;
		GLint m_hotspotLocation;
		GLint m_alphaLocation;
		GLint m_zIndexLocation;
		GLint m_textureLocation;
		GLint m_canvasFlipLocation;
		GLint m_spriteFilpLocation;

		ProgramObject m_program;
		SharedTexture m_texture;
	};

	//Provides additional extended functionality on top of Sprite2d

	class Sprite2dExt : public Sprite2d
	{
	protected:
		Sprite2dExt(); //Compatibility interface

	public:
		static ShaderObject *spVertexShader, *spFragmentShader;
		static CGEConstString getVertexString();
		static CGEConstString getFragmentString();
		
        Sprite2dExt(GLuint textureID, int width, int height);
        Sprite2dExt(const SharedTexture& texture);
		~Sprite2dExt();

		//Set scale ratio for texture tiling.
		void setScaleRatio(float sx, float sy)
		{
			m_program.bind();
			glUniform2f(m_scaleRatioLocation, sx, sy);
		}

		void setBlendColor(float r, float g, float b)
		{
			m_program.bind();
			m_program.sendUniformf(paramBlendColorName, r, g, b);
		}

		void setLineWidth(float lineWidth);
		void setLineColor(float r, float g, float b, float a = 1.0f);

		bool enableClip(bool flag);
		virtual bool clipZone();
		virtual void restoreZone();

		void setBlendMode(CGEGlobalBlendMode blendmode)
		{
			m_blendMode = blendmode;
		}

		//zone point value range: [-1.0, 1.0]
		//(-1.0, -1.0) represents the top-left corner; at least 3 zone-points are required to form a zone
		void pushZonePoint(const SharedPoint& pnt) { m_zone.push_back(pnt); }
		void pushZonePoint(float x, float y) { SharedPoint pnt(x, y); m_zone.push_back(pnt); }

		std::vector<SharedPoint>& getZone() { return m_zone; }
		void setZone(const std::vector<SharedPoint>& zone) { m_zone = zone; }
		void clearZone() { m_zone.clear(); }

//		virtual void render();

		void setGradient(float gradient);

		static void sSetZoneFlip(bool flipX, bool flipY)
		{
			sZoneFlip[0] = flipX ? -1.0f : 1.0f;
			sZoneFlip[1] = flipY ? -1.0f : 1.0f;
			if(sClipProgram != nullptr)
			{
				sClipProgram->bind();
				sClipProgram->sendUniformf("blankflip", sZoneFlip[0], sZoneFlip[1]);
			}
		}

		static Vec2f sZoneFlip;
		static void sReleaseClipProgram();
	private:
		bool _initProgram();
		bool _tryLinkWithStaticShader();
		void _clearProgram();

	protected:
		virtual void _drawFunc();

		GLint m_scaleRatioLocation;

		//Use a series of consecutive points to define a clipping region.
		//The first point is used as the closing point by default to form a closed region.
		//A zone requires at least three points
		std::vector<SharedPoint> m_zone; 
		std::vector<float> m_dataCache;	
		static ProgramObject* sClipProgram;		
		GeometryLineStrip2d* m_lineSprite;
		CGEGlobalBlendMode m_blendMode;
		float m_lineWidth;
	};

	class Sprite2dWithAlphaGradient : public Sprite2d
	{
	protected:
		Sprite2dWithAlphaGradient(); //Compatibility interface
	public:
		static ShaderObject *spVertexShader, *spFragmentShader;
		static CGEConstString getVertexString();
		static CGEConstString getFragmentString();
		
		Sprite2dWithAlphaGradient(GLuint textureID, int width, int height);
		Sprite2dWithAlphaGradient(const SharedTexture& texture);
		~Sprite2dWithAlphaGradient();

		void setAlphaTexture(const SharedTexture& tex) { m_texAlpha = tex; }

		void setAlphaFactor(float start, float end);

	private:
		bool _initProgram();
		bool _tryLinkWithStaticShader();
		void _clearProgram();

	protected:
		virtual void _drawFunc();

		static CGEConstString paramAlphaFactorName;
		static CGEConstString paramTexAlphaName;
		
		GLint m_texAlphaLocation, m_alphaFactorLocation;

		SharedTexture m_texAlpha;
	};

	//////////////////////////////////////////////////////////////////////////

	class Sprite2dInterChange : public Sprite2d
	{
	protected:
		Sprite2dInterChange(); //Compatibility interface

	public:
		static ShaderObject *spVertexShader, *spFragmentShader;
		static CGEConstString getVertexString();
		static CGEConstString getFragmentString();

		Sprite2dInterChange(GLuint textureID, int width, int height);
		Sprite2dInterChange(const SharedTexture& texture);
		~Sprite2dInterChange();

		//viewArea value range [0, 1]
		inline void setViewArea(const Vec4f& viewArea)
		{
			m_program.bind();
			glUniform4f(m_viewAreaLocation, viewArea[0], viewArea[1], viewArea[2], viewArea[3]);
		}

	private:
		bool _initProgram();
		bool _tryLinkWithStaticShader();
		void _clearProgram();

	protected:
//		virtual void _drawFunc();

		static CGEConstString paramViewAreaName;
		GLuint m_viewAreaLocation;
	};

	//Provides an effect similar to a GIF display.  Requires a "large" texture composed of multiple small images,
	//then specifies the texture region for each frame and updates according to the set region and time interval.
	class Sprite2dInterChangeExt : public Sprite2dInterChange
	{
	protected:
		Sprite2dInterChangeExt() { assert(0); } //Compatibility interface, should not be called
	public:
		Sprite2dInterChangeExt(GLuint textureID, int width, int height) : Sprite2dInterChange(textureID, width, height), m_frameIndex(0), m_deltaTime(100.0f), m_deltaAccum(0.0f), m_lastTime(0.0f), m_blendMode(CGEGLOBAL_BLEND_NONE), m_shouldLoop(true) {}
		Sprite2dInterChangeExt(const SharedTexture& texture) : Sprite2dInterChange(texture), m_frameIndex(0), m_deltaTime(100.0f), m_deltaAccum(0.0f), m_lastTime(0.0f), m_blendMode(CGEGLOBAL_BLEND_NONE), m_shouldLoop(true) {}
		~Sprite2dInterChangeExt() {}

		void firstFrame();
		void nextFrame(unsigned int offset = 1);

		void updateFrame(double dt); //Update based on delta time between frames

		void setFrameTime(double t) { m_lastTime = t; } //Set the starting total time, typically the current time
		void updateByTime(double t); //Update based on total elapsed time

		void setFPS(float fps) { m_deltaTime = 1000.0f / fps; } //Set the frame rate for sprite switching, default 10 fps

		//viewArea single component value range [0, 1]
		void pushViewArea(const Vec4f& area) { m_vecFrames.push_back(area); }
		void flushViewArea();
		void enableLoop(bool loop) { m_shouldLoop = loop; }

		void setBlendMode(CGEGlobalBlendMode blendMode) { m_blendMode = blendMode; }

        inline bool isLastFrame() { return m_frameIndex >= m_vecFrames.size() - 1; }
        inline void setToLastFrame() { m_frameIndex = (unsigned int)m_vecFrames.size() - 1; }
        
	protected:
		void _drawFunc();

		std::vector<Vec4f> m_vecFrames;
		unsigned int m_frameIndex;
		double m_deltaTime, m_deltaAccum, m_lastTime;
		CGEGlobalBlendMode m_blendMode;
		bool m_shouldLoop;
	};
    
    class Sprite2dInterChangeMultiple : public Sprite2dInterChange
    {
    protected:
        Sprite2dInterChangeMultiple() { assert(0); } //Compatibility interface
    public:
        Sprite2dInterChangeMultiple(int width, int height);
        ~Sprite2dInterChangeMultiple();
        
        
        typedef struct SpriteFrame
        {
            Vec4f frame;
            GLuint texture; //Texture id reference here; do not delete directly.
        }SpriteFrame;
        
        typedef struct FrameTexture
        {
            GLuint textureID;
            int width, height; //Actual texture width and height
            int col, row; //Column and row count of elements in the texture.
            int count; //Total number of elements in the texture; must satisfy count <= col * row.
        }FrameTexture;
        
        void nextFrame(unsigned int offset = 1);
        
        void updateFrame(double dt); //Update based on time interval
        
        void setCurrentTime(double t) { m_lastTime = t; }
        
        void updateByTime(double t); //Update based on total elapsed time
        
        void setFrameDelayTime(double dt) { m_deltaTime = dt; }
        
        void jumpToFrame(int frameIndex);
        void jumpToLastFrame();
        inline bool isFirstFrame() { return m_frameIndex == 0; }
        inline bool isLastFrame() { return m_frameIndex == m_vecFrames.size() - 1; }
        
        //Indices must be kept consistent.
        void setFrameTextures(const std::vector<FrameTexture>& vec);
        void setFrameTextures(FrameTexture* frames, int count);
        
        void enableLoop(bool loop) { m_shouldLoop = loop; }
        
		inline size_t currentFrame() { return m_frameIndex; }
		inline size_t totalFrames() { return m_vecFrames.size(); }

    protected:
        void _clearTextures();
        
        void _setToFrame(const SpriteFrame& frame);
        
//        void _drawFunc();
        void _calcFrames();
        
        std::vector<FrameTexture> m_vecTextures;
        
        std::vector<SpriteFrame> m_vecFrames;
        GLuint m_frameIndex;
        double m_deltaTime, m_deltaAccum, m_lastTime;
        bool m_shouldLoop;
    };

    //////////////////////////////////////////////////////////////////////////
    
    class Sprite2dSequence : public Sprite2d
    {
    protected:
        Sprite2dSequence() {assert(0); }
    public:
        Sprite2dSequence(int width, int height) : Sprite2d(-1, width, height), m_frameIndex(0), m_deltaTime(100.0), m_deltaAccum(0.0), m_lastTime(0.0), m_shouldLoop(true), m_canUpdate(true) {}

        ~Sprite2dSequence();
        
        void firstFrame();
        virtual void nextFrame(unsigned int offset = 1);
        
        virtual void updateFrame(double dt); //Update based on delta time between frames
        
        void setFrameTime(double t) { m_lastTime = t; } //Set the starting total time, typically the current time
        void updateByTime(double t); //Update based on total elapsed time
        size_t getFrameCount () {return m_frameTextures.size();}
        
        //useSec: true means seconds are used; false means milliseconds; default is false
        void setFPS(double fps, bool useSec = false); //Set the frame rate for sprite switching, default 10 fps
        double getCurrentTime(){return (double)m_frameIndex * m_deltaTime ;}
        inline void setFrameDuring(double during) { m_deltaTime = during; }
        
        inline void enableLoop(bool loop) { m_shouldLoop = loop; }
        
        virtual bool isLastFrame();
        virtual void setToLastFrame();
        
        inline unsigned int getFrameIndex() { return m_frameIndex; }
        
        inline void addFrameTexture(GLuint texID) { m_frameTextures.push_back(texID); }
        inline void setFrameTextures(const std::vector<GLuint> v) { m_frameTextures = v; }
        inline std::vector<GLuint>& getFrameTextures() { return m_frameTextures; }
        
        inline void setUpdate(bool update) { m_canUpdate = update; }
        inline bool canUpdate() { return m_canUpdate; }
        
    protected:
        void _drawFunc();
        
        unsigned int m_frameIndex;
        std::vector<GLuint> m_frameTextures;
        double m_deltaTime, m_deltaAccum, m_lastTime;
        bool m_shouldLoop, m_canUpdate;
    };
    
	//////////////////////////////////////////////////////////////////////////

	//Provides logic methods for slideshow
	class Sprite2dWith3dSpaceHelper : virtual public SpriteInterface2d
	{
	public:
		Sprite2dWith3dSpaceHelper() : m_rotMatrix(Mat3::makeIdentity()) {}

		using SpriteInterface2d::rotate;
		using SpriteInterface2d::rotateTo;

		void rotate(float rot, float x, float y, float z)
		{
			m_rotMatrix.rotate(rot, x, y, z);
		}

		void rotate(float rot, const Vec3f& axis)
		{
			m_rotMatrix.rotate(rot, axis[0], axis[1], axis[2]);
		}

		void rotateX(float rot)
		{
			m_rotMatrix.rotateX(rot);
		}

		void rotateY(float rot)
		{
			m_rotMatrix.rotateY(rot);
		}

		void rotateZ(float rot)
		{
			m_rotMatrix.rotateZ(rot);
		}

		void rotateTo(float rot, float x, float y, float z)
		{
			m_rotMatrix = Mat3::makeRotation(rot, x, y, z);
		}

		void rotateTo(float rot, const Vec3f& axis)
		{
			m_rotMatrix = Mat3::makeRotation(rot, axis[0], axis[1], axis[2]);
		}

		void rotateXTo(float rot)
		{
			m_rotMatrix = Mat3::makeXRotation(rot);
		}

		void rotateYTo(float rot)
		{
			m_rotMatrix = Mat3::makeYRotation(rot);
		}

		void rotateZTo(float rot)
		{
			m_rotMatrix = Mat3::makeZRotation(rot);
		}

		inline Mat3 getRotationMatrix() { return m_rotMatrix; }

	protected:
		CGE::Mat3 m_rotMatrix;
	};

	//////////////////////////////////////////////////////////////////////////

	class Sprite2dWith3dSpace : public Sprite2d
	{
	protected:
		Sprite2dWith3dSpace(); //Compatibility interface
	public:
		static ShaderObject *spVertexShader, *spFragmentShader;
		static CGEConstString getVertexString();
		static CGEConstString getFragmentString();

		Sprite2dWith3dSpace(GLuint textureID, int width, int height);
		Sprite2dWith3dSpace(const SharedTexture& texture);
		~Sprite2dWith3dSpace();

		void render();

		void rotate(float rot, float x, float y, float z)
		{
			m_rotMatrix.rotate(rot, x, y, z);
		}

		void rotate(float rot, const Vec3f& axis)
		{
			m_rotMatrix.rotate(rot, axis[0], axis[1], axis[2]);
		}

		void rotateX(float rot)
		{
			m_rotMatrix.rotateX(rot);
		}

		void rotateY(float rot)
		{
			m_rotMatrix.rotateY(rot);
		}

		void rotateZ(float rot)
		{
			m_rotMatrix.rotateZ(rot);
		}

		void rotateTo(float rot, float x, float y, float z)
		{
			m_rotMatrix = Mat3::makeRotation(rot, x, y, z);
		}

		void rotateTo(float rot, const Vec3f& axis)
		{
			m_rotMatrix = Mat3::makeRotation(rot, axis[0], axis[1], axis[2]);
		}

		void rotateXTo(float rot)
		{
			m_rotMatrix = Mat3::makeXRotation(rot);
		}

		void rotateYTo(float rot)
		{
			m_rotMatrix = Mat3::makeYRotation(rot);
		}

		void rotateZTo(float rot)
		{
			m_rotMatrix = Mat3::makeZRotation(rot);
		}

		//virtual void setCanvasSize(int width, int height)
		//{
		//	Mat4 m = Mat4::makeOrtho(0.0f, (float)width, 0.0f, (float)height, -1e20f, 1e20f);
		//	m_program.bind();
		//	glUniformMatrix4fv(m_projectionLocation, 1, false, m[0]);
		//}

		//virtual void restoreCanvasSize()
		//{
		//	m_program.bind();
		//	glUniformMatrix4fv(m_projectionLocation, 1, false, sOrthoProjectionMatrix[0]);
		//}

		void setHotspot(float sx, float sy)
		{
			m_hotspot[0] = sx;
			m_hotspot[1] = sy;
			m_program.bind();
			glUniform3f(m_hotspotLocation, sx, sy, m_hotspotZ);
		}

		void setHotspot3d(float sx, float sy, float sz)
		{
			m_hotspot[0] = sx;
			m_hotspot[1] = sy;
			m_hotspotZ = sz;
			m_program.bind();
			glUniform3f(m_hotspotLocation, sx, sy, sz);
		}

		void setHotspot2Center()
		{
			m_hotspot[0] = 0.0f;
			m_hotspot[1] = 0.0f;
			m_hotspotZ = 0.0f;
			m_program.bind();
			glUniform3f(m_hotspotLocation, 0.0f, 0.0f, 0.0f);
		}

		void setMVPMatrix(const Mat4& matrix)
		{
			m_program.bind();
			glUniformMatrix4fv(m_projectionLocation, 1, false, matrix[0]);
		}

		inline Mat3 getRotationMatrix() { return m_rotMatrix; }

	private:
		bool _initProgram();
		bool _tryLinkWithStaticShader();
		void _clearProgram();

		void rotate(float dRot)	{ assert(0); } //This interface is not available
		void rotateTo(float rot) { assert(0); } //Same as above

	protected:
		CGE::Mat3 m_rotMatrix;
		float m_hotspotZ;
	};

}


#endif
