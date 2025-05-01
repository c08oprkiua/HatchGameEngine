#ifndef ENGINE_RENDERING_GRAPHICSFUNCTIONS
#define ENGINE_RENDERING_GRAPHICSFUNCTIONS

#include <Engine/Rendering/Texture.h>

struct SDL_Surface;
struct SDL_Rect;
struct Matrix4x4;
struct ISprite;

struct GraphicsFunctions {
	virtual void Init() = 0;
	virtual Uint32 GetWindowFlags() = 0;
	virtual void SetVSync(bool enable) = 0;
	virtual void BackendSetup() = 0;
	virtual void Dispose() = 0;

	virtual Texture* CreateTexture(Uint32 format, Uint32 access, Uint32 width, Uint32 height) = 0;
	virtual Texture* CreateTextureFromPixels(Uint32 width, Uint32 height, void* pixels, int pitch) = 0;
	virtual Texture* CreateTextureFromSurface(SDL_Surface* surface) = 0;
	virtual int LockTexture(Texture* texture, void** pixels, int* pitch) = 0;
	virtual int UpdateTexture(Texture* texture, SDL_Rect* src, void* pixels, int pitch) = 0;
	virtual int UpdateYUVTexture(Texture* texture,
		SDL_Rect* src,
		void* pixelsY,
		int pitchY,
		void* pixelsU,
		int pitchU,
		void* pixelsV,
		int pitchV) = 0;
	virtual bool CanSetTexturePalette() = 0;
	virtual int SetTexturePalette(Texture* texture, void* palette, unsigned numPaletteColors) = 0;
	virtual void UnlockTexture(Texture* texture) = 0;
	virtual void DisposeTexture(Texture* texture) = 0;

	virtual void UseShader(void* shader) = 0;
	virtual void SetTextureInterpolation(bool interpolate) = 0;
	virtual void SetUniformTexture(Texture* texture, int uniform_index, int slot) = 0;
	virtual void SetUniformF(int location, int count, float* values) = 0;
	virtual void SetUniformI(int location, int count, int* values) = 0;

	virtual void UpdateGlobalPalette() = 0;

	virtual void UpdateViewport() = 0;
	virtual void UpdateClipRect() = 0;
	virtual void UpdateOrtho(float left, float top, float right, float bottom) = 0;
	virtual void UpdatePerspective(float fovy, float aspect, float near, float far) = 0;
	virtual void UpdateProjectionMatrix() = 0;
	virtual void MakePerspectiveMatrix(Matrix4x4* out,
		float fov,
		float near,
		float far,
		float aspect) = 0;

	virtual void Clear() = 0;
	virtual void Present() = 0;
	virtual void SetRenderTarget(Texture* texture) = 0;
	virtual void ReadFramebuffer(void* pixels, int width, int height) = 0;
	virtual void UpdateWindowSize(int width, int height) = 0;

	virtual void SetBlendColor(float r, float g, float b, float a) = 0;
	virtual void SetBlendMode(int srcC, int dstC, int srcA, int dstA) = 0;
	virtual void SetTintColor(float r, float g, float b, float a) = 0;
	virtual void SetTintMode(int mode) = 0;
	virtual void SetTintEnabled(bool enabled) = 0;
	virtual void SetLineWidth(float n) = 0;

	virtual void StrokeLine(float x1, float y1, float x2, float y2) = 0;
	virtual void StrokeCircle(float x, float y, float rad, float thickness) = 0;
	virtual void StrokeEllipse(float x, float y, float w, float h) = 0;
	virtual void StrokeRectangle(float x, float y, float w, float h) = 0;
	virtual void FillCircle(float x, float y, float rad) = 0;
	virtual void FillEllipse(float x, float y, float w, float h) = 0;
	virtual void FillTriangle(float x1, float y1, float x2, float y2, float x3, float y3) = 0;
	virtual void FillRectangle(float x, float y, float w, float h) = 0;

	virtual void DrawTexture(Texture* texture,
		float sx,
		float sy,
		float sw,
		float sh,
		float x,
		float y,
		float w,
		float h) = 0;
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
		unsigned paletteID) = 0;
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
		unsigned paletteID) = 0;

	virtual void DrawPolygon3D(void* data,
		int vertexCount,
		int vertexFlag,
		Texture* texture,
		Matrix4x4* modelMatrix,
		Matrix4x4* normalMatrix) = 0;
	virtual void DrawSceneLayer3D(void* layer,
		int sx,
		int sy,
		int sw,
		int sh,
		Matrix4x4* modelMatrix,
		Matrix4x4* normalMatrix) = 0;
	virtual void DrawModel(void* model,
		Uint16 animation,
		Uint32 frame,
		Matrix4x4* modelMatrix,
		Matrix4x4* normalMatrix) = 0;
	virtual void DrawModelSkinned(void* model,
		Uint16 armature,
		Matrix4x4* modelMatrix,
		Matrix4x4* normalMatrix) = 0;
	virtual void DrawVertexBuffer(Uint32 vertexBufferIndex,
		Matrix4x4* modelMatrix,
		Matrix4x4* normalMatrix) = 0;
	virtual void BindVertexBuffer(Uint32 vertexBufferIndex) = 0;
	virtual void UnbindVertexBuffer() = 0;
	virtual void BindScene3D(Uint32 sceneIndex) = 0;
	virtual void ClearScene3D(Uint32 sceneIndex) = 0;
	virtual void DrawScene3D(Uint32 sceneIndex, Uint32 drawMode) = 0;

	virtual void* CreateVertexBuffer(Uint32 maxVertices) = 0;
	virtual void DeleteVertexBuffer(void* vtxBuf) = 0;
	virtual void MakeFrameBufferID(ISprite* sprite) = 0;
	virtual void DeleteFrameBufferID(ISprite* sprite) = 0;

	virtual void SetStencilEnabled(bool enabled) = 0;
	virtual bool IsStencilEnabled() = 0;
	virtual void SetStencilTestFunc(int stencilTest) = 0;
	virtual void SetStencilPassFunc(int stencilOp) = 0;
	virtual void SetStencilFailFunc(int stencilOp) = 0;
	virtual void SetStencilValue(int value) = 0;
	virtual void SetStencilMask(int mask) = 0;
	virtual void ClearStencil() = 0;

	virtual void SetDepthTesting(bool enabled) = 0;
};

#endif /* ENGINE_RENDERING_GRAPHICSFUNCTIONS */
