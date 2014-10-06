#include "Panel.h"

#include "Debug.h"

Panel::Panel()
	: m_xCurtain(0xdd, 100), m_bPanelEnabled(true), m_bPanelOpen(false) {

	m_xCurtain.SetRenderingLayer(Renderer::eRenderingLayerHud3);
}

Panel::~Panel() {
}

void Panel::Initialize() {
}

void Panel::OpenPanel() {
	if (m_bPanelOpen && m_xCurtain.IsClosed()) {
		// already open
		return;
	}
	
	if (!m_bPanelOpen && m_xCurtain.IsOpen()) {
		// show
		m_xCurtain.Close();
	}
}

bool Panel::IsPanelOpen() {
	return m_bPanelOpen;
}

void Panel::SetAlpha(uint32 alpha) {
	m_xCurtain.SetAlpha(alpha);
}

void Panel::SetFadeTime(uint32 fadetime) {
	m_xCurtain.SetFadeTime(fadetime);
}

void Panel::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xCurtain.Update(frame);

	if (!m_xCurtain.IsClosed() && m_bPanelOpen) {
		m_bPanelOpen = false;
//		SetButtonsEnabled(false);
	} else if (m_xCurtain.IsClosed() && !m_bPanelOpen) {
		m_bPanelOpen = true;
//		SetButtonsEnabled(true);
	}
}

void Panel::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xCurtain.Render(renderer, frame);
}
