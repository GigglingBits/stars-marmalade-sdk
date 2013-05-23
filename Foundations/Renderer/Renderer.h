#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <string>
#include "IwGx.h"
#include "IwGxFont.h"
#include "Viewport.h"
#include "Texture.h"
#include "VertexStreamWorld.h"
#include "VertexStreamScreen.h"

class Renderer {
public:
	enum RenderingLayer {
		eRenderingLayerBackground = -1,
		eRenderingLayerHorizonVeryFar = 8,
		eRenderingLayerHorizonFar = 7,
		eRenderingLayerHorizonClose = 6,
		eRenderingLayerStaticGameObjects = 5,
		eRenderingLayerDynamicGameObjects = 4,
		eRenderingLayerForegroundObjects = 3,
		eRenderingLayerTextEffects = 2,
		eRenderingLayerHud = 1
	};

	enum FontType {
		eFontTypeLarge = 0,
		eFontTypeNormal = 1,
		eFontTypeSmall = 2,
		eFontTypeSystem = 3,
		eFontTypeLast = 4
	};

private:
	CIwSVec2 m_xScreenOffset;
	Viewport m_xViewport;
	RenderingLayer m_eCurrentRenderingLayer;

	CIwGxFont* m_apxFonts[eFontTypeLast];

public:
	Renderer();
	~Renderer();

	void SetViewport(const Viewport& viewport);
	CIwFVec2 GetScreenCenterWorldSpace();

	void SetDefaultRederingLayer();
	void SetRederingLayer(RenderingLayer renderinglayer);

	void SetFonts(const std::string& large, const std::string& normal, const std::string& small, const std::string& system);

public:
	// drawing to world (world coordinates)
	void Draw(VertexStreamWorld& shape, Texture& texture);
	void DrawPolygon(CIwFVec2 vertices[], int count, uint32 framecol, uint32 bodycol);
	void DrawPolygon(CIwFVec2 vertices[], int count, uint32 framecols[], uint32 bodycols[]);
	void DrawPolygon(CIwFVec2 vertices[], int count, CIwTexture* image);
	void DrawLine(const CIwFVec2& startpos, const CIwFVec2& endpos, uint32 col = 0xff888888);
	void DrawLine(CIwFVec2 vertices[], int count, uint32 vertexcols[]);
	void DrawImage(CIwTexture* image, const CIwFVec2& pos, const CIwFVec2& size, bool flipped = false);
	void DrawImage(CIwTexture* image, CIwFVec2* vertices, int count, bool flipped = false);
	void DrawText(std::string text, const CIwFVec2& pos, FontType fonttype = eFontTypeNormal, uint32 col = 0xff888888);
	void DrawText(std::string text, const CIwFVec2& pos, const CIwFVec2& size, FontType fonttype = eFontTypeNormal, uint32 col = 0xff888888);

	void DebugDrawCoords(const CIwFVec2& point);

	// drawing to screen (pixel coordinates)
	void Draw(VertexStreamScreen& shape, Texture& texture);
	void DrawRect(const CIwRect& rect, uint32 framecol, uint32 bodycol);
	void DrawPolygon(CIwSVec2 vertices[], int count, uint32 framecol, uint32 bodycol);
	void DrawPolygon(CIwSVec2 vertices[], int count, uint32 framecols[], uint32 bodycols[]);
	void DrawPolygon(CIwSVec2 vertices[], int count, CIwTexture* image);
	void DrawLine(const CIwSVec2& startpos, const CIwSVec2& endpos, uint32 col = 0xff888888);
	void DrawImage(CIwTexture* image, const CIwSVec2& pos, const CIwSVec2& size, bool flipped = false);
	void DrawImage(CIwTexture* image, CIwSVec2* vertices, int count, bool flipped = false);
	void DrawText(std::string text, const CIwSVec2& pos, FontType fonttype = eFontTypeNormal, uint32 col = 0xff888888);
	void DrawText(std::string text, const CIwSVec2& pos, const CIwSVec2& size, FontType fonttype = eFontTypeNormal, uint32 col = 0xff888888);
	void DrawText(std::string text, const CIwRect& rect, FontType fonttype = eFontTypeNormal, uint32 col = 0xff888888);

	void DebugDrawTouch(const CIwSVec2& startpos, const CIwSVec2& endpos, uint32 col = 0xff888888);
	void DebugDrawCoords(const CIwSVec2& point);

private:
	// drawing to screen (sub-pixel coordinates)
	void DrawPolygonSubPixel(CIwSVec2 vertices[], int count, uint32 framecol, uint32 bodycol);
	void DrawPolygonSubPixel(CIwSVec2 vertices[], int count, CIwColour framecols[], CIwColour bodycols[]);
	void DrawPolygonSubPixel(CIwSVec2 vertices[], int count, CIwTexture* image, bool flipped);

	// drawing to screen (pixel coordinates)
	void DrawText(const char* text, const CIwRect& rect, FontType font, bool center, uint32 col);

	// generic helpers
	template <class TVertex>
	void DrawPolygon(TVertex vertices[], int count, CIwTexture* image);

	template <class TVertexStream, class TVertex>
	void DrawTexture(TVertexStream& shape, Texture& texture);

	// conversion to sub-pixel
	INLINE iwsfixed ConvertSFixedFromInt(int16 a);
	INLINE iwsfixed ConvertSFixedFromFloat(float a);

	CIwSVec2* CreatGxCacheVertexStream(CIwSVec2 vertices[], int count);
	CIwSVec2* CreatGxCacheVertexStream(CIwFVec2 vertices[], int count);
	CIwColour* CreatGxCacheColourStream(uint32 cols[], int count);

	// utility functions
	CIwMaterial* CreateGxCacheMaterial(CIwTexture* texture);
	CIwMaterial* CreateGxCacheMaterial(uint colour);
#if defined (IW_USE_LEGACY_MODULES)
	CIwSVec2* CreatGxCacheUvStream(CIwSVec2 vertices[], int count, CIwTexture* image);
#else 
	CIwFVec2* CreatGxCacheUvStream(CIwSVec2 vertices[], int count, CIwTexture* image);
#endif
	void CalculateAABB(CIwSVec2 vertices[], int count, CIwSVec2& topleft, CIwSVec2& bottomright);
};

#endif
