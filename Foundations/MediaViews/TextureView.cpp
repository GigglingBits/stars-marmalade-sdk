#include "TextureView.h"
#include "Debug.h"
#include "FactoryManager.h"

TextureView::TextureView(const std::string textureid) : m_pxTexture(NULL) {
	IW_CALLSTACK_SELF;
	m_pxTexture = FactoryManager::GetTextureFactory().Create(textureid);
}

TextureView::~TextureView() {
	if (m_pxTexture) {
		delete m_pxTexture;
	}
}

void TextureView::Initialize() {
}

void TextureView::OnDoLayout(const CIwSVec2& screensize) {
	m_xGemoetry.SetRect(GetPosition().x, GetPosition().y, GetSize().x, GetSize().y);
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
