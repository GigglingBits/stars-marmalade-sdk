#include "LevelHudText.h"
#include "Debug.h"
#include "FactoryManager.h"

LevelHudText::LevelHudText() : m_pxBackground(NULL), m_eFont(Renderer::eFontTypeNormal), m_uiColour(0xffffffff) {
}

LevelHudText::~LevelHudText() {
	if (m_pxBackground) {
		delete m_pxBackground;
	}
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
	if (m_pxBackground) {
		delete m_pxBackground;
	}
	m_pxBackground = FactoryManager::GetTextureFactory().Create(texturename);
	if (m_pxBackground) {
		SetBackgroundShape();
	}
}

void LevelHudText::OnDoLayout(const CIwSVec2& screensize) {
	IW_CALLSTACK_SELF;
	if (m_pxBackground) {
		SetBackgroundShape();
	}
}

void LevelHudText::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;
	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
	}
}

void LevelHudText::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (m_pxBackground) {
		renderer.Draw (m_xBackgroundShape, *m_pxBackground);
	}
	
	renderer.DrawText(
		m_sText,
		GetPosition(),
		m_eFont,
		m_uiColour);
}

void LevelHudText::SetBackgroundShape() {
	m_xBackgroundShape.SetRect(GetPosition());
}

