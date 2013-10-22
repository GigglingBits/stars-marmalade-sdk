#include "TextureView.h"
#include "Debug.h"

TextureView::TextureView(Texture* texture) : m_pxTexture(NULL) {
	m_pxTexture = texture;
}

TextureView::~TextureView() {
	if (m_pxTexture) {
		delete m_pxTexture;
	}
}

void TextureView::Initialize() {
}

void TextureView::OnDoLayout(const CIwSVec2& screensize) {
	m_xGemoetry.SetRect(GetPosition().x, GetPosition().y, GetPosition().w, GetPosition().h);
}

void TextureView::OnUpdate(const FrameData& frame) {
	if (m_pxTexture) {
		m_pxTexture->Update(frame.GetAvgRealDurationMs());
	}
}

void TextureView::OnRender(Renderer& renderer, const FrameData& frame) {
	if (m_pxTexture) {
		renderer.Draw(m_xGemoetry, *m_pxTexture);
	}
}
