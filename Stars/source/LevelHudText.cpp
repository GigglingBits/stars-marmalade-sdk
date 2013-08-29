#include "LevelHudText.h"
#include "Debug.h"
#include "FactoryManager.h"

LevelHudText::LevelHudText() {
}

LevelHudText::~LevelHudText() {
}

void LevelHudText::Initialize() {
	SetRederingLayer(Renderer::eRenderingLayerHud);
}

void LevelHudText::SetPosition(const CIwSVec2 pos) {
	m_xPosition = pos;
	InvalidateLayout();
}

void LevelHudText::SetSize(const CIwSVec2 size) {
	m_xSize = size;
	InvalidateLayout();
}

void LevelHudText::SetText(const std::string &s) {
	m_sText = s;
}

void LevelHudText::OnDoLayout(const CIwSVec2& screensize) {
	IW_CALLSTACK_SELF;
}

void LevelHudText::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;
}

void LevelHudText::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;
	
	renderer.DrawText(
		m_sText,
		m_xPosition,
		m_xSize,
		Renderer::eFontTypeLarge,
		0xffffffff);
}
