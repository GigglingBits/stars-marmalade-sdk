#include "Hud.h"
#include "Debug.h"
#include "FactoryManager.h"

Hud::Hud(GameFoundation& game) :
	m_rxGame(game),
	m_xBuffs(game),
	m_bIsEnabled(true) {
}

void Hud::Initialize() {
	IW_CALLSTACK_SELF;
	m_xVial.Initialize();
	m_xBuffs.Initialize();
}

void Hud::SetEnabled(bool enabled) {
	m_xBuffs.SetEnabled(enabled);
	m_bIsEnabled = enabled;
}

void Hud::ClearBuffs() {
	m_xBuffs.ClearBuffs();
}

void Hud::OnDoLayout(const CIwSVec2& screensize) {
	int extent = GetScreenExtents();
	
	int spacing = extent / 40;
	
	int dustvialheight = extent / 3;
	int dustvialwidth = extent / 7;
	
	// dust vial
	int x = screensize.x - dustvialwidth - spacing;
	int y = screensize.y - dustvialheight - spacing;
	int w = dustvialwidth;
	int h = dustvialheight;
	CIwRect rect(x, y, w, h);
	m_xVial.SetPosition(rect);
}

void Hud::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xVial.SetDustAmount(m_rxGame.GetDustFillPercent(), m_rxGame.GetDustQueuedPercent());
	m_xVial.Update(frame);

	m_xBuffs.Update(frame);
}

void Hud::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;
	
	if (!m_bIsEnabled) {
		return;
	}

	m_xVial.Render(renderer, frame);
	m_xBuffs.Render(renderer, frame);
}
