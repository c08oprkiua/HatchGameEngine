#ifndef ENGINE_RENDERING_SOFTWARE_SOFTWARERENDERER_H
#define ENGINE_RENDERING_SOFTWARE_SOFTWARERENDERER_H

#include <Engine/Includes/HashMap.h>
#include <Engine/Includes/Standard.h>
#include <Engine/Includes/StandardSDL2.h>
#include <Engine/Math/Clipper.h>
#include <Engine/Math/Matrix4x4.h>
#include <Engine/Rendering/Material.h>
#include <Engine/Rendering/Software/Contour.h>
#include <Engine/Rendering/Software/SoftwareEnums.h>
#include <Engine/Rendering/Texture.h>
#include <Engine/Rendering/GraphicsFunctions.h>
#include <Engine/Rendering/VertexBuffer.h>
#include <Engine/ResourceTypes/IModel.h>
#include <Engine/ResourceTypes/ISprite.h>

typedef int (*MultCalcFunc)(int x, int y);

template<MultCalcFunc comp_func>
struct MulTable {
	int table[0x10000];

	constexpr MulTable<comp_func>(): table() {
		for (int alpha = 0; alpha < 0x100; alpha++) {
			for (int color = 0; color < 0x100; color++) {
				table[alpha << 8 | color] = comp_func(alpha, color);
			}
		}
	}

    constexpr int operator[](int i) const {
        if (i > 0 && i < sizeof table){
            return table[i];
        }
        return 0;
    }
};

constexpr int RegCalc(int x, int y){
	return (x * y) >> 8;
}

constexpr int InvCalc(int x, int y){
    return ((x ^ 0xFF) * y) >> 8;
}

constexpr int SubCalc(int x, int y){
    return (x * -(y ^ 0xFF)) >> 8;
}



class SoftwareRenderer : public GraphicsFunctions {
private:
	static void SetColor(Uint32 color);
	static Uint32 GetBlendColor();
	static bool SetupPolygonRenderer(Matrix4x4* modelMatrix, Matrix4x4* normalMatrix);
	static void InitContour(Contour* contourBuffer, int dst_y1, int scanLineCount);
	static void RasterizeCircle(int ccx,
		int ccy,
		int dst_x1,
		int dst_y1,
		int dst_x2,
		int dst_y2,
		float rad,
		Contour* contourBuffer);
	void StrokeThickCircle(float x, float y, float rad, float thickness);
	static void DrawShapeTextured(Texture* texturePtr,
		unsigned numPoints,
		float* px,
		float* py,
		int* pc,
		float* pu,
		float* pv);

public:
	static GraphicsFunctions *BackendFunctions;
	static Uint32 CompareColor;
	static TileScanLine TileScanLineBuffer[MAX_FRAMEBUFFER_HEIGHT];
	static Sint32 SpriteDeformBuffer[MAX_FRAMEBUFFER_HEIGHT];
	static bool UseSpriteDeform;
	static Contour ContourBuffer[MAX_FRAMEBUFFER_HEIGHT];
	//static int MultTable[0x10000];
	//static int MultTableInv[0x10000];
	//static int MultSubTable[0x10000];
	constexpr static MulTable<RegCalc> MultTable = MulTable<RegCalc>();
	constexpr static MulTable<InvCalc> MultTableInv = MulTable<InvCalc>();
	constexpr static MulTable<SubCalc> MultSubTable = MulTable<SubCalc>();

	static void RenderStart();
	static void RenderEnd();

	static void NewPixelFunction(Uint32 src, Uint32* dst, BlendState& state);

	virtual void Init();
	virtual Uint32 GetWindowFlags();
	virtual void SetVSync(bool enable){}
	virtual void BackendSetup();
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
	static void SetFilter(int filter);
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
	virtual void ReadFramebuffer(void* pixels, int width, int height);
	virtual void UpdateWindowSize(int width, int height);

	virtual void SetBlendColor(float r, float g, float b, float a);
	virtual void SetBlendMode(int srcC, int dstC, int srcA, int dstA);
	virtual void SetTintColor(float r, float g, float b, float a);
	virtual void SetTintMode(int mode);
	virtual void SetTintEnabled(bool enabled);

	static void Resize(int width, int height);
	static void SetClip(float x, float y, float width, float height);
	static void ClearClip();
	static void Save();
	static void Translate(float x, float y, float z);
	static void Rotate(float x, float y, float z);
	static void Scale(float x, float y, float z);
	static void Restore();
	static int ConvertBlendMode(int blendMode);
	static BlendState GetBlendState();
	static bool AlterBlendState(BlendState& state);
 	static void PixelNoFiltSetOpaque(Uint32* src,
 		Uint32* dst,
 		BlendState& state,
 		int* multTableAt,
 		int* multSubTableAt);
// 	static void PixelNoFiltSetTransparent(Uint32* src,
// 		Uint32* dst,
// 		BlendState& state,
// 		int* multTableAt,
// 		int* multSubTableAt);
// 	static void PixelNoFiltSetAdditive(Uint32* src,
// 		Uint32* dst,
// 		BlendState& state,
// 		int* multTableAt,
// 		int* multSubTableAt);
// 	static void PixelNoFiltSetSubtract(Uint32* src,
// 		Uint32* dst,
// 		BlendState& state,
// 		int* multTableAt,
// 		int* multSubTableAt);
// 	static void PixelNoFiltSetMatchEqual(Uint32* src,
// 		Uint32* dst,
// 		BlendState& state,
// 		int* multTableAt,
// 		int* multSubTableAt);
// 	static void PixelNoFiltSetMatchNotEqual(Uint32* src,
// 		Uint32* dst,
// 		BlendState& state,
// 		int* multTableAt,
// 		int* multSubTableAt);
// 	static void PixelTintSetOpaque(Uint32* src,
// 		Uint32* dst,
// 		BlendState& state,
// 		int* multTableAt,
// 		int* multSubTableAt);
// 	static void PixelTintSetTransparent(Uint32* src,
// 		Uint32* dst,
// 		BlendState& state,
// 		int* multTableAt,
// 		int* multSubTableAt);
// 	static void PixelTintSetAdditive(Uint32* src,
// 		Uint32* dst,
// 		BlendState& state,
// 		int* multTableAt,
// 		int* multSubTableAt);
// 	static void PixelTintSetSubtract(Uint32* src,
// 		Uint32* dst,
// 		BlendState& state,
// 		int* multTableAt,
// 		int* multSubTableAt);
// 	static void PixelTintSetMatchEqual(Uint32* src,
// 		Uint32* dst,
// 		BlendState& state,
// 		int* multTableAt,
// 		int* multSubTableAt);
// 	static void PixelTintSetMatchNotEqual(Uint32* src,
// 		Uint32* dst,
// 		BlendState& state,
// 		int* multTableAt,
// 		int* multSubTableAt);
// 	static void SetTintFunction(int blendFlags);

	virtual void SetLineWidth(float n);

	virtual void StrokeLine(float x1, float y1, float x2, float y2);
	virtual void StrokeCircle(float x, float y, float rad, float thickness);
	virtual void StrokeEllipse(float x, float y, float w, float h);
	virtual void StrokeRectangle(float x, float y, float w, float h);
	virtual void FillCircle(float x, float y, float rad);
	virtual void FillEllipse(float x, float y, float w, float h);
	virtual void FillTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
	virtual void FillRectangle(float x, float y, float w, float h);
	static void FillTriangleBlend(float x1,
		float y1,
		float x2,
		float y2,
		float x3,
		float y3,
		int c1,
		int c2,
		int c3);
	static void
	FillQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
	static void FillQuadBlend(float x1,
		float y1,
		float x2,
		float y2,
		float x3,
		float y3,
		float x4,
		float y4,
		int c1,
		int c2,
		int c3,
		int c4);

	static void DrawTriangleTextured(Texture* texturePtr,
		float x1,
		float y1,
		float x2,
		float y2,
		float x3,
		float y3,
		int c1,
		int c2,
		int c3,
		float u1,
		float v1,
		float u2,
		float v2,
		float u3,
		float v3);
	static void DrawQuadTextured(Texture* texturePtr,
		float x1,
		float y1,
		float x2,
		float y2,
		float x3,
		float y3,
		float x4,
		float y4,
		int c1,
		int c2,
		int c3,
		int c4,
		float u1,
		float v1,
		float u2,
		float v2,
		float u3,
		float v3,
		float u4,
		float v4);
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
	static void DrawTile(int tile, int x, int y, bool flipX, bool flipY);
	static void DrawSceneLayer_InitTileScanLines(SceneLayer* layer, View* currentView);
	static void DrawSceneLayer_HorizontalParallax(SceneLayer* layer, View* currentView);
	static void DrawSceneLayer_VerticalParallax(SceneLayer* layer, View* currentView);
	static void DrawSceneLayer_CustomTileScanLines(SceneLayer* layer, View* currentView);
	static void DrawSceneLayer(SceneLayer* layer,
		View* currentView,
		int layerIndex,
		bool useCustomFunction);

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
	virtual void DrawModel(void* model,
		Uint16 animation,
		Uint32 frame,
		Matrix4x4* modelMatrix,
		Matrix4x4* normalMatrix);
	virtual void DrawModelSkinned(void* model,
		Uint16 armature,
		Matrix4x4* modelMatrix,
		Matrix4x4* normalMatrix);
	virtual void DrawVertexBuffer(Uint32 vertexBufferIndex,
		Matrix4x4* modelMatrix,
		Matrix4x4* normalMatrix);
	virtual void BindVertexBuffer(Uint32 vertexBufferIndex);
	virtual void UnbindVertexBuffer();
	virtual void BindScene3D(Uint32 sceneIndex);
	virtual void ClearScene3D(Uint32 sceneIndex){}
	virtual void DrawScene3D(Uint32 sceneIndex, Uint32 drawMode);

	virtual void* CreateVertexBuffer(Uint32 maxVertices){return NULL;}
	virtual void DeleteVertexBuffer(void* vtxBuf){}
	virtual void MakeFrameBufferID(ISprite* sprite);
	virtual void DeleteFrameBufferID(ISprite* sprite){}

	virtual void SetStencilEnabled(bool enabled);
	virtual bool IsStencilEnabled();
	virtual void SetStencilTestFunc(int stencilTest);
	virtual void SetStencilPassFunc(int stencilOp);
	virtual void SetStencilFailFunc(int stencilOp);
	virtual void SetStencilValue(int value);
	virtual void SetStencilMask(int mask);
	virtual void ClearStencil();

	//static PixelFunction GetPixelFunction(int blendFlag);

// 	static void PixelStencil(Uint32* src,
// 		Uint32* dst,
// 		BlendState& state,
// 		int* multTableAt,
// 		int* multSubTableAt);
	static void SetDotMask(int mask);
	static void SetDotMaskH(int mask);
	static void SetDotMaskV(int mask);
	static void SetDotMaskOffsetH(int offset);
	static void SetDotMaskOffsetV(int offset);
// 	static void PixelDotMaskH(Uint32* src,
// 		Uint32* dst,
// 		BlendState& state,
// 		int* multTableAt,
// 		int* multSubTableAt);
// 	static void PixelDotMaskV(Uint32* src,
// 		Uint32* dst,
// 		BlendState& state,
// 		int* multTableAt,
// 		int* multSubTableAt);
// 	static void PixelDotMaskHV(Uint32* src,
// 		Uint32* dst,
// 		BlendState& state,
// 		int* multTableAt,
// 		int* multSubTableAt);

	virtual void SetDepthTesting(bool enabled){}

};


#endif /* ENGINE_RENDERING_SOFTWARE_SOFTWARERENDERER_H */
