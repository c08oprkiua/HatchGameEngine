#ifndef ENGINE_RENDERING_GL_GLRENDERER_H
#define ENGINE_RENDERING_GL_GLRENDERER_H

#include <Engine/Includes/HashMap.h>
#include <Engine/Includes/Standard.h>
#include <Engine/Includes/StandardSDL2.h>
#include <Engine/Math/Matrix4x4.h>
#include <Engine/Rendering/GL/GLShader.h>
#include <Engine/Rendering/GL/GLShaderBuilder.h>
#include <Engine/Rendering/GL/GLShaderContainer.h>
#include <Engine/Rendering/GL/Includes.h>
#include <Engine/Rendering/Texture.h>
#include <Engine/Rendering/GraphicsFunctions.h>
#include <Engine/ResourceTypes/ISprite.h>

class GLRenderer : public GraphicsFunctions {
public:
	static SDL_GLContext Context;
	static GLShader* CurrentShader;
	static GLShaderContainer* ShaderShape;
	static GLShaderContainer* ShaderShape3D;
	static GLShaderContainer* ShaderFogLinear;
	static GLShaderContainer* ShaderFogExp;
	static GLShaderContainer* ShaderYUV;
	static GLint DefaultFramebuffer;
	static GLint DefaultRenderbuffer;
	static GLuint BufferCircleFill;
	static GLuint BufferCircleStroke;
	static GLuint BufferSquareFill;

	virtual void Init();
	virtual Uint32 GetWindowFlags();
	virtual void SetVSync(bool enabled);
	virtual void BackendSetup(){}
	virtual void Dispose();

	virtual Texture* CreateTexture(Uint32 format, Uint32 access, Uint32 width, Uint32 height);
	virtual int LockTexture(Texture* texture, void** pixels, int* pitch);
	virtual int UpdateTexture(Texture* texture, SDL_Rect* src, void* pixels, int pitch);
	virtual int UpdateYUVTexture(Texture* texture,
		SDL_Rect* src,
		void* pixelsY,
		int pitchY,
		void* pixelsU,
		int pitchU,
		void* pixelsV,
		int pitchV);
	virtual void UnlockTexture(Texture* texture);
	virtual void DisposeTexture(Texture* texture);
	virtual void SetRenderTarget(Texture* texture);
	virtual void ReadFramebuffer(void* pixels, int width, int height);
	virtual void UpdateWindowSize(int width, int height);
	virtual void UpdateViewport();
	virtual void UpdateClipRect();
	virtual void UpdateOrtho(float left, float top, float right, float bottom);
	virtual void UpdatePerspective(float fovy, float aspect, float nearv, float farv);
	virtual void UpdateProjectionMatrix();
	virtual void
	MakePerspectiveMatrix(Matrix4x4* out, float fov, float near, float far, float aspect);
	static void SUseShader(void *shader);
	virtual void UseShader(void* shader);
	virtual void SetUniformF(int location, int count, float* values);
	virtual void SetUniformI(int location, int count, int* values);
	virtual void SetUniformTexture(Texture* texture, int uniform_index, int slot);
	virtual void UpdateGlobalPalette();
	virtual void Clear();
	virtual void Present();
	virtual void SetBlendColor(float r, float g, float b, float a);
	static void SSetBlendMode(int srcC, int dstC, int srcA, int dstA);
	virtual void SetBlendMode(int srcC, int dstC, int srcA, int dstA);
	virtual void SetTintColor(float r, float g, float b, float a);
	virtual void SetTintMode(int mode);
	virtual void SetTintEnabled(bool enabled);
	virtual void SetLineWidth(float n);
	virtual void StrokeLine(float x1, float y1, float x2, float y2);
	virtual void StrokeCircle(float x, float y, float rad, float thickness);
	virtual void StrokeEllipse(float x, float y, float w, float h);
	virtual void StrokeRectangle(float x, float y, float w, float h);
	virtual void FillCircle(float x, float y, float rad);
	virtual void FillEllipse(float x, float y, float w, float h);
	virtual void FillTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
	virtual void FillRectangle(float x, float y, float w, float h);
	static Uint32 CreateTexturedShapeBuffer(float* data, int vertexCount);
	static void DrawTexturedShapeBuffer(Texture* texture, Uint32 bufferID, int vertexCount);
	virtual void DrawTexture(Texture* texture,
		float sx,
		float sy,
		float sw,
		float sh,
		float x,
		float y,
		float w,
		float h);
	virtual void DrawSprite(ISprite* sprite,
		int animation,
		int frame,
		int x,
		int y,
		bool flipX,
		bool flipY,
		float scaleW,
		float scaleH,
		float rotation,
		unsigned paletteID);
	virtual void DrawSpritePart(ISprite* sprite,
		int animation,
		int frame,
		int sx,
		int sy,
		int sw,
		int sh,
		int x,
		int y,
		bool flipX,
		bool flipY,
		float scaleW,
		float scaleH,
		float rotation,
		unsigned paletteID);
	virtual void DrawPolygon3D(void* data,
		int vertexCount,
		int vertexFlag,
		Texture* texture,
		Matrix4x4* modelMatrix,
		Matrix4x4* normalMatrix);
	virtual void DrawSceneLayer3D(void* layer,
		int sx,
		int sy,
		int sw,
		int sh,
		Matrix4x4* modelMatrix,
		Matrix4x4* normalMatrix);
	virtual void DrawModel(void* inModel,
		Uint16 animation,
		Uint32 frame,
		Matrix4x4* modelMatrix,
		Matrix4x4* normalMatrix);
	virtual void DrawModelSkinned(void* inModel,
		Uint16 armature,
		Matrix4x4* modelMatrix,
		Matrix4x4* normalMatrix);
	virtual void
	DrawVertexBuffer(Uint32 vertexBufferIndex, Matrix4x4* modelMatrix, Matrix4x4* normalMatrix);
	virtual void BindVertexBuffer(Uint32 vertexBufferIndex);
	virtual void UnbindVertexBuffer();
	virtual void ClearScene3D(Uint32 sceneIndex);
	virtual void DrawScene3D(Uint32 sceneIndex, Uint32 drawMode);
	virtual void* CreateVertexBuffer(Uint32 maxVertices);
	virtual void DeleteVertexBuffer(void* vtxBuf);
	virtual void MakeFrameBufferID(ISprite* sprite);
	virtual void DeleteFrameBufferID(ISprite* sprite);
	virtual void SetDepthTesting(bool enable);
};

#endif /* ENGINE_RENDERING_GL_GLRENDERER_H */
