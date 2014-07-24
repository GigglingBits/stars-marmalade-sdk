#include "HudText.h"
#include "Debug.h"
#include "FactoryManager.h"

HudText::HudText() : m_eFont(Renderer::eFontTypeNormal), m_uiColour(0xffffffff), m_eAlignH(Renderer::eTextAlignHLeft), m_eAlignV(Renderer::eTextAlignVBottom) {
}

void HudText::Initialize() {
	SetRenderingLayer(Renderer::eRenderingLayerHud);
}

void HudText::SetFont(Renderer::FontType font) {
	m_eFont = font;
}

void HudText::SetColour(uint32 col) {
	m_uiColour = col;
}

void HudText::SetText(const std::string &s) {
	m_sText = s;
}

void HudText::SetAlignH(Renderer::TextAlignmentHorizontal align) {
	m_eAlignH = align;
}

void HudText::SetAlignV(Renderer::TextAlignmentVertical align) {
	m_eAlignV = align;
}

void HudText::SetBackground(const std::string& texturename) {
	IW_CALLSTACK_SELF;
	Window::SetBackground(FactoryManager::GetTextureFactory().Create(texturename));
}

void HudText::OnUpdate(const FrameData& frame) {
}

void HudText::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.DrawText(
		m_sText,
		GetPosition(),
		m_eFont,
		m_uiColour,
		m_eAlignH,
		m_eAlignV);
}

