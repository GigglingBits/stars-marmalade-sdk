#include "Curtain.h"
#include "Debug.h"

Curtain::Curtain(uint32 alpha, uint32 fadetime) {
	m_eState = eStateOpen;
	m_iCountdown = 0;
	m_uiAlpha = alpha;
	m_uiFadeTime = fadetime;
	
	SetRenderingLayer(Renderer::eRenderingLayerHudFront);
}

bool Curtain::IsClosed() {
	return m_eState == eStateClosed;
}

bool Curtain::IsOpen() {
	return m_eState == eStateOpen;
}

void Curtain::Close() {
	m_eState = eStateClosing;
	m_iCountdown = m_uiFadeTime;
}

void Curtain::Open() {
	m_eState = eStateOpening;
	m_iCountdown = m_uiFadeTime;
}

void Curtain::SetAlpha(uint32 alpha) {
	m_uiAlpha = alpha;
}

void Curtain::SetFadeTime(uint32 fadetime) {
	m_uiFadeTime = fadetime;
}

void Curtain::OnUpdate(const FrameData& frame) {
	if (m_eState == eStateOpening) {
		m_iCountdown -= frame.GetRealDurationMs();
		if (m_iCountdown <= 0) {
			m_eState = eStateOpen;
		}

	} else if (m_eState == eStateClosing) {
		m_iCountdown -= frame.GetRealDurationMs();
		if (m_iCountdown <= 0) {
			m_eState = eStateClosed;
		}
	}
}

void Curtain::OnRender(Renderer& renderer, const FrameData& frame) {
	if (m_eState == eStateOpen) {
		return;
	}

	// shape
	const CIwSVec2& screensize = frame.GetScreensize();
	CIwRect screenarea(0, 0, screensize.x, screensize.y);
	
	// opacity
	uint32 alpha = m_uiAlpha;
	if (m_eState != eStateClosed) {
		alpha *= m_iCountdown;
		alpha /= m_uiFadeTime;
		if (m_eState == eStateClosing) {
			alpha = m_uiAlpha - alpha; // invert alpha
		}
	}

	// draw the curtain
	uint32 colour = 0x00000000 | (alpha << 24);
	renderer.DrawRect(screenarea, 0x00000000, colour);
}
