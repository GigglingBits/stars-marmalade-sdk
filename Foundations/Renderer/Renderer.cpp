#include "Debug.h"
#include "Renderer.h"
#include "IwGxFont.h"

Renderer::Renderer() {
	m_xScreenOffset = CIwSVec2::g_Zero;
	m_eCurrentRenderingLayer = eRenderingLayerDynamicGameObjects;

	memset(m_apxFonts, 0, sizeof(m_apxFonts));
	
	IwGetResManager()->LoadGroup("fonts.group");
}

Renderer::~Renderer() {
	IwGetResManager()->DestroyGroup("fonts");
}

void Renderer::SetFonts(const std::string& large, const std::string& normal, const std::string& small, const std::string& system) {

	CIwGxFont* font;
	font = (CIwGxFont*)IwGetResManager()->GetResNamed(large.c_str(), "CIwGxFont");
	m_apxFonts[eFontTypeLarge] = font;

	font = (CIwGxFont*)IwGetResManager()->GetResNamed(normal.c_str(), "CIwGxFont");
	m_apxFonts[eFontTypeNormal] = font;

	font = (CIwGxFont*)IwGetResManager()->GetResNamed(small.c_str(), "CIwGxFont");
	m_apxFonts[eFontTypeSmall] = font;

	font = (CIwGxFont*)IwGetResManager()->GetResNamed(system.c_str(), "CIwGxFont");
	m_apxFonts[eFontTypeSystem] = font;

	IwGxFontSetFont(font);
}

CIwMaterial* Renderer::CreateGxCacheMaterial(CIwTexture* texture) {
	CIwMaterial* mat = IW_GX_ALLOC_MATERIAL();
	mat->SetColAmbient(0xffffffff);
	mat->SetModulateMode(CIwMaterial::MODULATE_NONE);
	mat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	mat->SetTexture(texture);
	return mat;
}

CIwMaterial* Renderer::CreateGxCacheMaterial(uint colour) {
	CIwMaterial* mat = IW_GX_ALLOC_MATERIAL();
	mat->SetColAmbient(colour);
	return mat;
}

CIwFVec2 Renderer::GetScreenCenterWorldSpace() {
	// where in the world is the screen center?
	CIwSVec2 screencenter = m_xViewport.GetViewportSize();
	screencenter.x /= 2; 
	screencenter.y /= 2; 

	// how far away is the world center from the screen center?
	return m_xViewport.ViewToWorld(screencenter);
}

void Renderer::SetViewport(const Viewport& viewport) {
	m_xViewport = viewport;
	m_xScreenOffset = m_xViewport.GetScreenViewOffset();
}

void Renderer::SetDefaultRederingLayer() {
	SetRederingLayer(eRenderingLayerDynamicGameObjects);
}

void Renderer::SetRederingLayer(RenderingLayer renderinglayer) {
	m_eCurrentRenderingLayer = renderinglayer;
}

void Renderer::Draw(VertexStreamWorld& shape, Texture& texture) {
	DrawTexture<VertexStreamWorld, CIwFVec2>(shape, texture);
}

void Renderer::Draw(VertexStreamScreen& shape, Texture& texture) {
	DrawTexture<VertexStreamScreen, CIwSVec2>(shape, texture);
}

template <class TVertexStream, class TVertex>
void Renderer::DrawTexture(TVertexStream& shape, Texture& texture) {
	IW_CALLSTACK_SELF;

	if (shape.GetVertCount() <= 2) {
		// IwAssertMsg(MYAPP, false, ("Shape is not a valid polygon"));
		return;
	}

	if (texture.IsImage()) {
		DrawImage(
			texture.GetImage(), 
			shape.GetVerts(), 
			shape.GetVertCount(), 
			texture.GetHorizontalFlip());
	} else if (texture.IsPattern()) {
		DrawPolygon(
			shape.GetVerts(), 
			shape.GetVertCount(), 
			texture.GetPattern());
	} else if (texture.IsColour()) {
		uint32 col = texture.GetColour();
		TVertex* verts = shape.GetVerts();
		int count = shape.GetVertCount();

		// closed or open poly?
		bool isclosed = (verts[0] == verts[count - 1]);
		DrawPolygon(
			verts, 
			count,
			isclosed ? 0x00000000 : col,
			isclosed ? col : 0x00000000);
	} 
}

void Renderer::DrawRect(const CIwRect& rect, uint32 framecol, uint32 bodycol) {
	IW_CALLSTACK_SELF;

	IwGxSetScreenSpaceOrg(&CIwSVec2::g_Zero);
	const int count = 5;

	CIwSVec2 vertices[count];
	vertices[0].x = rect.x, vertices[0].y = rect.y;
	vertices[1].x = rect.x, vertices[1].y = rect.y + rect.h;
	vertices[2].x = rect.x + rect.w, vertices[2].y = rect.y + rect.h;
	vertices[3].x = rect.x + rect.w, vertices[3].y = rect.y;
	vertices[4].x = rect.x, vertices[4].y = rect.y;

	CIwSVec2* polystream = CreatGxCacheVertexStream(vertices, count);
	DrawPolygonSubPixel(polystream, count, framecol, bodycol);
}

void Renderer::DrawImage(CIwTexture* image, const CIwFVec2& pos, const CIwFVec2& size, bool flipped) {
	IW_CALLSTACK_SELF;

	IwGxSetScreenSpaceOrg(&m_xScreenOffset);
	const int count = 4;
	
	CIwFVec2 vertices[count];
	vertices[0].x = pos.x, vertices[0].y = pos.y;
	vertices[1].x = pos.x + size.x, vertices[1].y = pos.y;
	vertices[2].x = pos.x + size.x, vertices[2].y = pos.y + size.y;
	vertices[3].x = pos.x, vertices[3].y = pos.y + size.y;

	CIwSVec2* polystream = CreatGxCacheVertexStream((CIwFVec2*)vertices, count);	
	DrawPolygonSubPixel(polystream, count, image, flipped);
}

void Renderer::DrawImage(CIwTexture* image, CIwFVec2* vertices, int count, bool flipped) {
	IW_CALLSTACK_SELF;

	IwGxSetScreenSpaceOrg(&m_xScreenOffset);

	CIwSVec2* polystream = CreatGxCacheVertexStream(vertices, count);	
	DrawPolygonSubPixel(polystream, count, image, flipped);
}

void Renderer::DrawImage(CIwTexture* image, CIwSVec2* vertices, int count, bool flipped) {
	IW_CALLSTACK_SELF;

	IwGxSetScreenSpaceOrg(&CIwSVec2::g_Zero);

	CIwSVec2* polystream = CreatGxCacheVertexStream(vertices, count);	
	DrawPolygonSubPixel(polystream, count, image, flipped);
}

void Renderer::DrawPolygon(CIwSVec2 vertices[], int count, CIwTexture* image) {
	DrawPolygon<CIwSVec2>(vertices, count, image);
}

void Renderer::DrawPolygon(CIwFVec2 vertices[], int count, CIwTexture* image) {
	DrawPolygon<CIwFVec2>(vertices, count, image);
}

template <class TVertex>
void Renderer::DrawPolygon(TVertex vertices[], int count, CIwTexture* image) {
	IW_CALLSTACK_SELF;

	IwGxSetScreenSpaceOrg(&m_xScreenOffset);

	CIwSVec2* polystream = CreatGxCacheVertexStream(vertices, count);	
#if defined (IW_USE_LEGACY_MODULES)
	CIwSVec2* wrapuv = CreatGxCacheUvStream(polystream, count, image);	
#else 
	CIwFVec2* wrapuv = CreatGxCacheUvStream(polystream, count, image);	
#endif
	IwGxSetUVStream(wrapuv);

	IwGxSetColStream(NULL);

	IwGxSetMaterial(CreateGxCacheMaterial(image));

	IwGxSetVertStreamScreenSpaceSubPixel(polystream, count);

	int32 slot = IwGxGetScreenSpaceSlot();
	IwGxSetScreenSpaceSlot((int32)m_eCurrentRenderingLayer);
	IwGxDrawPrims(IW_GX_TRI_FAN, NULL, count);
	IwGxSetScreenSpaceSlot(slot);
}

void Renderer::DrawPolygonSubPixel(CIwSVec2 vertices[], int count, CIwTexture* image, bool flipped) {
	IW_CALLSTACK_SELF;

#if defined (IW_USE_LEGACY_MODULES)
	// standard orientation
	static const CIwSVec2 standarduv[4] = {
		CIwSVec2(0 << 12, 1 << 12),
		CIwSVec2(1 << 12, 1 << 12),
		CIwSVec2(1 << 12, 0 << 12),
		CIwSVec2(0 << 12, 0 << 12),
	};

	// horizontally flipped orientation
	static const CIwSVec2 flippeduv[4] = {
		CIwSVec2(1 << 12, 1 << 12),
		CIwSVec2(0 << 12, 1 << 12),
		CIwSVec2(0 << 12, 0 << 12),
		CIwSVec2(1 << 12, 0 << 12),
	};

	IwGxSetUVStream((CIwSVec2*)(flipped ? flippeduv : standarduv));
#else
	// standard orientation
	static const CIwFVec2 standarduv[4] = {
		CIwFVec2(0.0f, 1.0f),
		CIwFVec2(1.0f, 1.0f),
		CIwFVec2(1.0f, 0.0f),
		CIwFVec2(0.0f, 0.0f),
	};

	// horizontally flipped orientation
	static const CIwFVec2 flippeduv[4] = {
		CIwFVec2(1.0f, 1.0f),
		CIwFVec2(0.0f, 1.0f),
		CIwFVec2(0.0f, 0.0f),
		CIwFVec2(1.0f, 0.0f),
	};

	IwGxSetUVStream((CIwFVec2*)(flipped ? flippeduv : standarduv));
#endif

	IwGxSetColStream(NULL);

	IwGxSetMaterial(CreateGxCacheMaterial(image));

	IwGxSetVertStreamScreenSpaceSubPixel(vertices, count);

	int32 slot = IwGxGetScreenSpaceSlot();
	IwGxSetScreenSpaceSlot((int32)m_eCurrentRenderingLayer);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, count);
	IwGxSetScreenSpaceSlot(slot);
}

void Renderer::DrawPolygon(CIwFVec2 vertices[], int count, uint32 framecol, uint32 bodycol) {
	IW_CALLSTACK_SELF;

	if (!vertices || count == 0) {
		IwAssertMsg(MYAPP, false, ("Cannot render empty polygon"));
		return;
	}

	IwGxSetScreenSpaceOrg(&m_xScreenOffset);

	CIwSVec2* polystream = CreatGxCacheVertexStream(vertices, count);
	DrawPolygonSubPixel(polystream, count, framecol, bodycol);
}

void Renderer::DrawPolygon(CIwFVec2 vertices[], int count, uint32 framecols[], uint32 bodycols[]) {
	IW_CALLSTACK_SELF;

	if (!vertices || count == 0) {
		IwAssertMsg(MYAPP, false, ("Cannot render empty polygon"));
		return;
	}

	IwGxSetScreenSpaceOrg(&m_xScreenOffset);

	CIwSVec2* polystream = CreatGxCacheVertexStream(vertices, count);
	CIwColour* framecolstream = framecols ? CreatGxCacheColourStream(framecols, count) : NULL;
	CIwColour* bodycolstream = bodycols ? CreatGxCacheColourStream(bodycols, count) : NULL;
	DrawPolygonSubPixel(polystream, count, framecolstream, bodycolstream);
}

void Renderer::DrawPolygon(CIwSVec2 vertices[], int count, uint32 framecol, uint32 bodycol) {
	IW_CALLSTACK_SELF;

	if (!vertices || count == 0) {
		IwAssertMsg(MYAPP, false, ("Cannot render empty polygon"));
		return;
	}

	IwGxSetScreenSpaceOrg(&CIwSVec2::g_Zero);

	CIwSVec2* polystream = CreatGxCacheVertexStream(vertices, count);
	DrawPolygonSubPixel(polystream, count, framecol, bodycol);
}

void Renderer::DrawPolygon(CIwSVec2 vertices[], int count, uint32 framecols[], uint32 bodycols[]) {
	IW_CALLSTACK_SELF;

	if (!vertices || count == 0) {
		IwAssertMsg(MYAPP, false, ("Cannot render empty polygon"));
		return;
	}

	IwGxSetScreenSpaceOrg(&CIwSVec2::g_Zero);

	CIwSVec2* polystream = CreatGxCacheVertexStream(vertices, count);
	CIwColour* framecolstream = framecols ? CreatGxCacheColourStream(framecols, count) : NULL;
	CIwColour* bodycolstream = bodycols ? CreatGxCacheColourStream(bodycols, count) : NULL;
	DrawPolygonSubPixel(polystream, count, framecolstream, bodycolstream);
}

void Renderer::DrawPolygonSubPixel(CIwSVec2 vertices[], int count, uint32 framecol, uint32 bodycol) {
	IW_CALLSTACK_SELF;

	if (!(bodycol || framecol)) {
		return;
	}

	IwGxSetUVStream(NULL);
	IwGxSetColStream(NULL);

	// set stream once, use it for 2 primitive drawing instructions
	IwGxSetVertStreamScreenSpaceSubPixel(vertices, count);

	int32 slot = IwGxGetScreenSpaceSlot();
	IwGxSetScreenSpaceSlot((int32)m_eCurrentRenderingLayer);
	if (bodycol) {
	    IwGxSetMaterial(CreateGxCacheMaterial(bodycol));
		IwGxDrawPrims(IW_GX_TRI_FAN, NULL, count);
	}
	if (framecol) {
	    IwGxSetMaterial(CreateGxCacheMaterial(framecol));
		IwGxDrawPrims(IW_GX_LINE_STRIP, NULL, count);
	}
	IwGxSetScreenSpaceSlot(slot);
}

void Renderer::DrawPolygonSubPixel(CIwSVec2 vertices[], int count, CIwColour framecols[], CIwColour bodycols[]) {
	IW_CALLSTACK_SELF;

	IwGxSetUVStream(NULL);

	IwGxSetVertStreamScreenSpaceSubPixel(vertices, count);

	int32 slot = IwGxGetScreenSpaceSlot();
	IwGxSetScreenSpaceSlot((int32)m_eCurrentRenderingLayer);

	//CIwMaterial* mat = IW_GX_ALLOC_MATERIAL();
	//IwGxSetMaterial(mat);
	IwGxSetMaterial(CreateGxCacheMaterial(0xffffffff));
	
	if (bodycols) {
		IwGxSetColStream(bodycols, count);
		IwGxDrawPrims(IW_GX_TRI_FAN, NULL, count);
	}
	if (framecols) {
		IwGxSetColStream(framecols, count);
		IwGxDrawPrims(IW_GX_LINE_STRIP, NULL, count);
	}

	IwGxSetScreenSpaceSlot(slot);
}

void Renderer::DrawLine(const CIwFVec2& startpos, const CIwFVec2& endpos, uint32 col) {
	IW_CALLSTACK_SELF;

	CIwFVec2 vertices[2] = {startpos, endpos};
	DrawPolygon(vertices, 2, col, 0);
}

void Renderer::DrawLine(CIwFVec2 vertices[], int count, uint32 vertexcols[]) {
	IW_CALLSTACK_SELF;

	DrawPolygon(vertices, count, vertexcols, NULL);
}

void Renderer::DrawLine(const CIwSVec2& startpos, const CIwSVec2& endpos, uint32 col) {
	IW_CALLSTACK_SELF;

	CIwSVec2 vertices[2] = {startpos, endpos};
	DrawPolygon(vertices, 2, col, 0);
}

void Renderer::DrawImage(CIwTexture* image, const CIwSVec2& pos, const CIwSVec2& size, bool flipped) {
	IW_CALLSTACK_SELF;

	IwGxSetScreenSpaceOrg(&CIwSVec2::g_Zero);
	const int count = 4;
	
	CIwSVec2 vertices[count];
	vertices[0].x = pos.x, vertices[0].y = pos.y + size.y;
	vertices[1].x = pos.x + size.x, vertices[1].y = pos.y + size.y;
	vertices[2].x = pos.x + size.x, vertices[2].y = pos.y;
	vertices[3].x = pos.x, vertices[3].y = pos.y;

	CIwSVec2* polystream = CreatGxCacheVertexStream((CIwSVec2*)vertices, count);	
	DrawPolygonSubPixel(polystream, count, image, flipped);
}

void Renderer::DrawText(std::string text, const CIwRect& rect, FontType fonttype, uint32 col) {
	IW_CALLSTACK_SELF;

	IwGxSetScreenSpaceOrg(&CIwSVec2::g_Zero);

	DrawText(text.c_str(), rect, fonttype, true, col);
}

void Renderer::DrawText(std::string text, const CIwSVec2& pos, const CIwSVec2& size, FontType fonttype, uint32 col) {
	IW_CALLSTACK_SELF;

	DrawText(text, CIwRect(pos.x, pos.y, size.x, size.y), fonttype, col);
}

void Renderer::DrawText(std::string text, const CIwSVec2& pos, FontType fonttype, uint32 col) {
	IW_CALLSTACK_SELF;

	IwGxSetScreenSpaceOrg(&CIwSVec2::g_Zero);

	DrawText(text.c_str(), CIwRect(pos.x, pos.y, 250, 50), fonttype, false, col);
}

void Renderer::DrawText(std::string text, const CIwFVec2& pos, const CIwFVec2& size, FontType fonttype, uint32 col) {
	IW_CALLSTACK_SELF;

	IwGxSetScreenSpaceOrg(&m_xScreenOffset);

	CIwSVec2 screenpos = m_xViewport.WorldToScreen(pos);
	CIwSVec2 screensize = m_xViewport.WorldToScreen(size);
	DrawText(text.c_str(), CIwRect(screenpos.x, screenpos.y, screensize.x, screensize.y), fonttype, true, col);
}

void Renderer::DrawText(std::string text, const CIwFVec2& pos, FontType fonttype, uint32 col) {
	IW_CALLSTACK_SELF;

	IwGxSetScreenSpaceOrg(&m_xScreenOffset);

	CIwSVec2 screenpos = m_xViewport.WorldToScreen(pos);
	DrawText(text.c_str(), CIwRect(screenpos.x, screenpos.y, 100, 50), fonttype, false, col);
}

void Renderer::DrawText(const char* text, const CIwRect& rect, FontType font, bool center, uint32 col) {
	IW_CALLSTACK_SELF;

	if (center) {
		IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_CENTRE);
		IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_MIDDLE);
	} else {
		IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_LEFT);
		IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_BOTTOM);
	}

	if (m_apxFonts[font]) {
		IwAssertMsg(MYAPP, m_apxFonts[font], ("Cannot draw text because font %i is not set. Please load the fonts before drawing text.", font));
		IwGxFontSetFont(m_apxFonts[font]);
	}

	IwGxFontSetRect(rect);
	IwGxFontSetCol(col);

	int32 slot = IwGxGetScreenSpaceSlot();
	IwGxSetScreenSpaceSlot((int32)m_eCurrentRenderingLayer);
	IwGxFontDrawText(text);
	IwGxSetScreenSpaceSlot(slot);
}

void Renderer::DebugDrawTouch(const CIwSVec2& startpos, const CIwSVec2& endpos, uint32 col) {
#ifdef IW_DEBUG
	IW_CALLSTACK_SELF;
	DrawLine(startpos, endpos, col);
#endif
}

void Renderer::DebugDrawCoords(const CIwSVec2& point) {
#ifdef IW_DEBUG
	IW_CALLSTACK_SELF;

	const int len = 20;
	char buf[len];
	snprintf(buf, len, "(%i/%i)", point.x, point.y);
	DrawText(std::string(buf), point, eFontTypeSmall);

	uint32 col = 0xffffffff;
	CIwSVec2 line[2] = { point, point };
	line[0] = point;
	line[0].x -= 10;
	line[1] = point;
	line[1].x += 10;
	DrawLine(line[0], line[1], col);

	line[0] = point;
	line[0].y -= 10;
	line[1] = point;
	line[1].y += 10;
	DrawLine(line[0], line[1], col);
#endif
}

void Renderer::DebugDrawCoords(const CIwFVec2& point) {
#ifdef IW_DEBUG
	IW_CALLSTACK_SELF;

	const int len = 20;
	char buf[len];
	snprintf(buf, len, "(%.1f/%.1f)", point.x, point.y);
	DrawText(std::string(buf), point, eFontTypeSmall);

	uint32 cols[2] = { 0xffffffff, 0xffffffff };
	CIwFVec2 line[2] = { point, point };
	line[0] = point;
	line[0].x -= 1.0f;
	line[1] = point;
	line[1].x += 1.0f;
	DrawLine(line, 2, cols);

	line[0] = point;
	line[0].y -= 1.0f;
	line[1] = point;
	line[1].y += 1.0f;
	DrawLine(line, 2, cols);
#endif
}

iwsfixed Renderer::ConvertSFixedFromInt(int16 a) {
#ifdef IW_DEBUG
    int32 fix32 = ((int32)a) << 3;
    IwAssertMsgN(349,GEOM, fix32 >> 31 == fix32 >> 15, ("Conversion overflow in renderer. Graphics issues may occur."));
#endif
    return (iwsfixed)(a << 3);
}

iwsfixed Renderer::ConvertSFixedFromFloat(float a) {
#ifdef IW_DEBUG
    int32 fix32 = (int32)(a * (float)(1 << 3));
    IwAssertMsgN(349,GEOM, fix32 >> 31 == fix32 >> 15, ("Conversion overflow in renderer. Graphics issues may occur."));
#endif
    return (iwsfixed)(a * (float)(1 << 3));
}

CIwSVec2* Renderer::CreatGxCacheVertexStream(CIwSVec2 vertices[], int count) {
	IwAssertMsg(MYAPP, count > 0 && vertices, ("Trying to allocate empty stream. This should probably never happen?"));

	CIwSVec2* polystream = IW_GX_ALLOC(CIwSVec2, count);
	for (int i = 0; i < count; i++) {
		polystream[i].x = ConvertSFixedFromInt(vertices[i].x);
		polystream[i].y = ConvertSFixedFromInt(vertices[i].y);
	}
	return polystream;
}

CIwSVec2* Renderer::CreatGxCacheVertexStream(CIwFVec2 vertices[], int count){
	IwAssertMsg(MYAPP, count > 0 && vertices, ("Trying to allocate empty stream. This should probably never happen?"));

	CIwSVec2* polystream = IW_GX_ALLOC(CIwSVec2, count);
	for (int i = 0; i < count; i++) {
		CIwSVec2 v = m_xViewport.WorldToScreen(vertices[i]);
		polystream[i].x = ConvertSFixedFromInt(v.x);
		polystream[i].y = ConvertSFixedFromInt(v.y);	
	}
	return polystream;
}

CIwColour* Renderer::CreatGxCacheColourStream(uint32 cols[], int count) {
	IwAssertMsg(MYAPP, count > 0 && cols, ("Trying to allocate empty stream. This should probably never happen?"));

	CIwColour* gxcols = IW_GX_ALLOC(CIwColour, count);
	for (int i = 0; i < count; i++) {
		gxcols[i] = cols[i];
	}
	return gxcols;
}

#if defined (IW_USE_LEGACY_MODULES)
CIwSVec2* Renderer::CreatGxCacheUvStream(CIwSVec2 vertices[], int count, CIwTexture* image) {
#else
CIwFVec2* Renderer::CreatGxCacheUvStream(CIwSVec2 vertices[], int count, CIwTexture* image) {
#endif
// get aabb
	CIwSVec2 topleft(0, 0), bottomright(0, 0);
	CalculateAABB(vertices, count, topleft, bottomright);

	// assert that aabb does not exceed 15x15 tiles
	// (would actually 16x16, but we need 1 reserve for the alignment)
	IwAssertMsg(MYAPP, 
		bottomright.x - topleft.x < ConvertSFixedFromInt(15 * image->GetWidth()), 
		("Polygon is too wide for the texture. UV map cannot be calculated."));
	IwAssertMsg(MYAPP, 
		bottomright.y - topleft.y < ConvertSFixedFromInt(15 * image->GetHeight()), 
		("Polygon is too tall for the texture. UV map cannot be calculated."));

	// center of aabb
	int16 texturewidth = image->GetWidth();
	int16 textureheight = image->GetHeight();
	CIwSVec2 uvorigin(
		topleft.x + ((bottomright.x - topleft.x) / 2),
		topleft.y + ((bottomright.y - topleft.y) / 2));

	// align with tiles
	uvorigin.x -= uvorigin.x % ConvertSFixedFromInt(texturewidth);
	uvorigin.y -= uvorigin.y % ConvertSFixedFromInt(textureheight);

	// allocate UV cache and convert/rescale coordinates relative to aabb center and tile size
#if defined (IW_USE_LEGACY_MODULES)
	CIwSVec2* uvstream = IW_GX_ALLOC(CIwSVec2, count);
#else
	CIwFVec2* uvstream = IW_GX_ALLOC(CIwFVec2, count);
#endif
	for (int i = 0; i < count; i++) {
		// TODO: Clean up for better efficiency; I just needed this for debugging
		CIwSVec2 v(vertices[i] - uvorigin);
		float x = (float) v.x;
		float y = (float) v.y;
		x /= (1 << 3);
		y /= (1 << 3);
		x /= texturewidth;
		y /= textureheight;
#if defined (IW_USE_LEGACY_MODULES)
		uvstream[i].x = IW_FIXED_FROM_FLOAT(x);
		uvstream[i].y = IW_FIXED_FROM_FLOAT(y);
#else
		uvstream[i].x = x;
		uvstream[i].y = y;
#endif
	}
	return uvstream;
}

void Renderer::CalculateAABB(CIwSVec2 vertices[], int count, CIwSVec2& topleft, CIwSVec2& bottomright) {
	// this function works in screenspace
	if (count < 1) {
		return;
	}

	topleft = vertices[0];
	bottomright = vertices[0];

	for (int i = 1; i < count; i++) {
		topleft.x = std::min<int16>(vertices[i].x, topleft.x);
		topleft.y = std::min<int16>(vertices[i].y, topleft.y);
		bottomright.x = std::max<int16>(vertices[i].x, bottomright.x);
		bottomright.y = std::max<int16>(vertices[i].y, bottomright.y);
	}
}
