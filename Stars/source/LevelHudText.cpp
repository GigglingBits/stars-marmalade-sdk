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
		CIwSVec2(150, 50),
		CIwSVec2(250, 50),
		Renderer::eFontTypeLarge,
		0xffffffff);
}
