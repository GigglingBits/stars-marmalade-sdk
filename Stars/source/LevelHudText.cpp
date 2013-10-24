#include "LevelHudText.h"
#include "Debug.h"
#include "FactoryManager.h"

LevelHudText::LevelHudText() : m_eFont(Renderer::eFontTypeNormal), m_uiColour(0xffffffff) {
}

void LevelHudText::Initialize() {
	SetRederingLayer(Renderer::eRenderingLayerHud);
}

void LevelHudText::SetFont(Renderer::FontType font) {
	m_eFont = font;
}

void LevelHudText::SetColour(uint32 col) {
	m_uiColour = col;
}

void LevelHudText::SetText(const std::string &s) {
	m_sText = s;
}

void LevelHudText::SetBackground(const std::string& texturename) {
	Window::SetBackground(FactoryManager::GetTextureFactory().Create(texturename));
}

void LevelHudText::OnUpdate(const FrameData& frame) {
}

void LevelHudText::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.DrawText(
		m_sText,
		GetPosition(),
		m_eFont,
		m_uiColour);
}

