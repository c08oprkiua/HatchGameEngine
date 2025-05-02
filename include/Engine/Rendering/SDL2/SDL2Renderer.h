#ifndef ENGINE_RENDERING_SDL2_SDL2RENDERER_H
#define ENGINE_RENDERING_SDL2_SDL2RENDERER_H

#include <Engine/Includes/HashMap.h>
#include <Engine/Includes/Standard.h>
#include <Engine/Includes/StandardSDL2.h>
#include <Engine/Math/Matrix4x4.h>
#include <Engine/Rendering/Texture.h>
#include <Engine/Rendering/GraphicsFunctions.h>
#include <Engine/ResourceTypes/ISprite.h>

class SDL2Renderer : public GraphicsFunctions {
private:
	static SDL_BlendMode GetCustomBlendMode(int srcC, int dstC, int srcA, int dstA);

public:
	static float RenderScale;

	static void CopyScreen(void* pixels, int width, int height);
	static void GetMetalSize(int* width, int* height);

	virtual void Init();
	virtual Uint32 GetWindowFlags();
	virtual void SetVSync(bool enable);
	virtual void BackendSetup(){}
	virtual void Dispose();

	virtual Texture* CreateTexture(Uint32 format, Uint32 access, Uint32 width, Uint32 height);
	virtual Texture* CreateTextureFromPixels(Uint32 width, Uint32 height, void* pixels, int pitch){return NULL;}
	virtual Texture* CreateTextureFromSurface(SDL_Surface* surface){return NULL;}
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
	virtual int SetTexturePalette(Texture* texture, void* palette, unsigned numPaletteColors){return 0;}
	virtual void UnlockTexture(Texture* texture);
	virtual void DisposeTexture(Texture* texture);

	virtual void UseShader(void* shader);
	virtual void SetTextureInterpolation(bool interpolate){}
	virtual void SetUniformTexture(Texture* texture, int uniform_index, int slot);
	virtual void SetUniformF(int location, int count, float* values);
	virtual void SetUniformI(int location, int count, int* values);

	virtual void UpdateGlobalPalette(){}

	virtual void UpdateViewport();
	virtual void UpdateClipRect();
	virtual void UpdateOrtho(float left, float top, float right, float bottom);
	virtual void UpdatePerspective(float fovy, float aspect, float near, float far);
	virtual void UpdateProjectionMatrix();
	virtual void MakePerspectiveMatrix(Matrix4x4* out,
		float fov,
		float near,
		float far,
		float aspect);

	virtual void Clear();
	virtual void Present();
	virtual void SetRenderTarget(Texture* texture);
	virtual void ReadFramebuffer(void* pixels, int width, int height){}
	virtual void UpdateWindowSize(int width, int height);

	virtual void SetBlendColor(float r, float g, float b, float a);
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
		Matrix4x4* normalMatrix){}
	virtual void DrawSceneLayer3D(void* layer,
		int sx,
		int sy,
		int sw,
		int sh,
		Matrix4x4* modelMatrix,
		Matrix4x4* normalMatrix){}
	virtual void DrawModel(void* model,
		Uint16 animation,
		Uint32 frame,
		Matrix4x4* modelMatrix,
		Matrix4x4* normalMatrix){}
	virtual void DrawModelSkinned(void* model,
		Uint16 armature,
		Matrix4x4* modelMatrix,
		Matrix4x4* normalMatrix){}
	virtual void DrawVertexBuffer(Uint32 vertexBufferIndex,
		Matrix4x4* modelMatrix,
		Matrix4x4* normalMatrix){}
	virtual void BindVertexBuffer(Uint32 vertexBufferIndex){}
	virtual void UnbindVertexBuffer(){}
	virtual void BindScene3D(Uint32 sceneIndex){}
	virtual void ClearScene3D(Uint32 sceneIndex){}
	virtual void DrawScene3D(Uint32 sceneIndex, Uint32 drawMode){}

	virtual void* CreateVertexBuffer(Uint32 maxVertices){return NULL;}
	virtual void DeleteVertexBuffer(void* vtxBuf){}
	virtual void MakeFrameBufferID(ISprite* sprite);
	virtual void DeleteFrameBufferID(ISprite* sprite){}

	virtual void SetStencilEnabled(bool enabled){}
	virtual bool IsStencilEnabled(){return false;}
	virtual void SetStencilTestFunc(int stencilTest){}
	virtual void SetStencilPassFunc(int stencilOp){}
	virtual void SetStencilFailFunc(int stencilOp){}
	virtual void SetStencilValue(int value){}
	virtual void SetStencilMask(int mask){}
	virtual void ClearStencil(){}

	virtual void SetDepthTesting(bool enabled){}
};

#endif /* ENGINE_RENDERING_SDL2_SDL2RENDERER_H */
