#include "LevelHudText.h"
#include "Debug.h"
#include "FactoryManager.h"

LevelHudText::LevelHudText() : m_pxBackground(NULL) {
}

LevelHudText::~LevelHudText() {
	if (m_pxBackground) {
		delete m_pxBackground;
	}
}

void LevelHudText::Initialize() {
	SetRederingLayer(Renderer::eRenderingLayerHud);
}

void LevelHudText::SetPosition(const CIwSVec2 pos) {
	m_xPosition = pos;
	InvalidateLayout();
	if (m_pxBackground) {
		SetBackgroundShape();
	}
}

void LevelHudText::SetSize(const CIwSVec2 size) {
	m_xSize = size;
	InvalidateLayout();
	if (m_pxBackground) {
		SetBackgroundShape();
	}
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
		m_xPosition,
		m_xSize,
		Renderer::eFontTypeNormal,
		0xffffffff);
}

void LevelHudText::SetBackgroundShape() {
	m_xBackgroundShape.SetRect(m_xPosition.x, m_xPosition.y, m_xSize.x, m_xSize.y);
}

