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
	static void CopyScreen(void* pixels, int width, int height);
	virtual void UpdateWindowSize(int width, int height);
	virtual void UpdateViewport();
	virtual void UpdateClipRect();
	virtual void UpdateOrtho(float left, float top, float right, float bottom);
	virtual void UpdatePerspective(float fovy, float aspect, float nearv, float farv);
	virtual void UpdateProjectionMatrix();
	virtual void
	MakePerspectiveMatrix(Matrix4x4* out, float fov, float near, float far, float aspect);
	static void GetMetalSize(int* width, int* height);
	virtual void UseShader(void* shader);
	virtual void SetUniformF(int location, int count, float* values);
	virtual void SetUniformI(int location, int count, int* values);
	virtual void SetUniformTexture(Texture* texture, int uniform_index, int slot);
	virtual void Clear();
	virtual void Present();
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
	virtual void MakeFrameBufferID(ISprite* sprite);
};

#endif /* ENGINE_RENDERING_SDL2_SDL2RENDERER_H */
