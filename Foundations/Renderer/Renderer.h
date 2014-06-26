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
		eRenderingLayerHorizonVeryFar = 13,
		eRenderingLayerHorizonFar = 12,
		eRenderingLayerHorizonClose = 11,
		eRenderingLayerGameBackground = 10,
		eRenderingLayerGameStaticObjects = 9,
		eRenderingLayerGameDynamicObjects = 8,
		eRenderingLayerGameForeground = 7,
		eRenderingLayerGameTextEffects = 6,
		eRenderingLayerGameFront = 5,
		eRenderingLayerHud3 = 4,
		eRenderingLayerHud2 = 3,
		eRenderingLayerHud = 2,
		eRenderingLayerHudFront = 1
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

	void SetViewport(const Viewport& viewport);
	CIwFVec2 GetScreenCenterWorldSpace();

	void SetDefaultRederingLayer();
	void SetRenderingLayer(RenderingLayer renderinglayer);

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
	void DrawImage(CIwTexture* image, CIwFVec2 vertices[], int count, bool flipped = false);
	void DrawImage(CIwTexture* image, CIwFVec2 vertices[], CIwFVec2 uvs[], uint32 cols[], int count, bool additiveblend = false);
	void DrawText(std::string text, const CIwFVec2& pos, FontType fonttype = eFontTypeNormal, uint32 col = 0xff888888);
	void DrawText(std::string text, const CIwFVec2& pos, const CIwFVec2& size, FontType fonttype = eFontTypeNormal, uint32 col = 0xff888888);

	void DebugDrawCoords(const CIwFVec2& point, uint32 colour = 0xffffffff, float size = 1.0f);

	// drawing to screen (pixel coordinates)
	void Draw(VertexStreamScreen& shape, Texture& texture);
	void DrawRect(const CIwRect& rect, uint32 framecol, uint32 bodycol);
	void DrawPolygon(CIwSVec2 vertices[], int count, uint32 framecol, uint32 bodycol);
	void DrawPolygon(CIwSVec2 vertices[], int count, uint32 framecols[], uint32 bodycols[]);
	void DrawPolygon(CIwSVec2 vertices[], int count, CIwTexture* image);
	void DrawLine(const CIwSVec2& startpos, const CIwSVec2& endpos, uint32 col = 0xff888888);
	void DrawImage(CIwTexture* image, const CIwSVec2& pos, const CIwSVec2& size, bool flipped = false);
	void DrawImage(CIwTexture* image, CIwSVec2 vertices[], int count, bool flipped = false);
	void DrawImage(CIwTexture* image, CIwSVec2 vertices[], CIwFVec2 uvs[], uint32 cols[], int count, bool additiveblend = false);
	void DrawText(std::string text, const CIwSVec2& pos, FontType fonttype = eFontTypeNormal, uint32 col = 0xff888888);
	void DrawText(std::string text, const CIwSVec2& pos, const CIwSVec2& size, FontType fonttype = eFontTypeNormal, uint32 col = 0xff888888);
	void DrawText(std::string text, const CIwRect& rect, FontType fonttype = eFontTypeNormal, uint32 col = 0xff888888);

	void DebugDrawTouch(const CIwSVec2& startpos, const CIwSVec2& endpos, uint32 col = 0xff888888);
	void DebugDrawCoords(const CIwSVec2& point, uint32 colour = 0xffffffff, int16 size = 10);

private:
	// create Gx cached streams
	CIwFVec2* CreatGxCacheVertexStream(CIwSVec2 vertices[], int count);
	CIwFVec2* CreatGxCacheVertexStream(CIwFVec2 vertices[], int count);
	CIwColour* CreatGxCacheColourStream(uint32 cols[], int count);
	CIwMaterial* CreateGxCacheMaterial(CIwTexture* texture, bool additivebelnding = false);
	CIwMaterial* CreateGxCacheMaterial(uint colour);
	CIwFVec2* CreatGxCacheTiledUvStream(CIwFVec2 vertices[], int count, CIwTexture* image);
	CIwFVec2* CreatGxCacheUvStream(CIwFVec2 uvs[], int count);
	
	// drawing to screen (sub-pixel coordinates)
	void DrawPolygonSubPixel(CIwFVec2 vertices[], int count, uint32 framecol, uint32 bodycol);
	void DrawPolygonSubPixel(CIwFVec2 vertices[], int count, CIwColour framecols[], CIwColour bodycols[]);
	void DrawPolygonSubPixel(CIwFVec2 vertices[], int count, CIwTexture* image, bool flipped);
	void DrawImageSubPixel(CIwMaterial* image, CIwFVec2 vertices[], CIwFVec2 uvs[], CIwColour cols[], int count);

	// drawing to screen (pixel coordinates)
	void DrawText(const char* text, const CIwRect& rect, FontType font, bool center, uint32 col);

	// generic helpers
	template <class TVertex>
	void DrawPolygon(TVertex vertices[], int count, CIwTexture* image);

	template <class TVertexStream, class TVertex>
	void DrawTexture(TVertexStream& shape, Texture& texture);

	// utility functions
	void CalculateAABB(CIwFVec2 vertices[], int count, CIwFVec2& topleft, CIwFVec2& bottomright);
};

#endif
