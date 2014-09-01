#include "Hud.h"
#include "Debug.h"
#include "FactoryManager.h"

Hud::Hud() :
	m_bIsEnabled(true) {
}

void Hud::Initialize() {
	IW_CALLSTACK_SELF;
	m_xVial.Initialize();
	m_xBuffs.Initialize();
	m_xLives.Initialize();
	m_xLives.SetNumber(0);
	m_xLives.SetFont(Renderer::eFontTypeLarge);
	m_xLives.SetColour(GAME_COLOUR_FONT_MAIN);
	m_xLives.SetAlignH(Renderer::eTextAlignHCenter);
	m_xLives.SetAlignV(Renderer::eTextAlignVMiddle);
}

void Hud::SetEnabled(bool enabled) {
	m_xBuffs.SetEnabled(enabled);
	m_bIsEnabled = enabled;
}

HudNumber& Hud::GetLives() {
	return m_xLives;
}

LevelDustVial& Hud::GetDustVial() {
	return m_xVial;
}

HudBuffPanel& Hud::GetBuffPanel() {
	return m_xBuffs;
}

void Hud::OnDoLayout(const CIwSVec2& screensize) {
	int extent = GetScreenExtents();
	
	int spacing = extent / 40;

	// lives
	int liveswidth = extent / 20;
	
	int x = spacing;
	int y = spacing;
	int w = liveswidth;
	int h = w;
	CIwRect rect(x, y, w, h);
	m_xLives.SetPosition(rect);
	
	// dust vial
	int dustvialheight = extent / 3;
	int dustvialwidth = extent / 7;
	
	x = screensize.x - dustvialwidth - spacing;
	y = screensize.y - dustvialheight - spacing;
	w = dustvialwidth;
	h = dustvialheight;
	rect.Make(x, y, w, h);
	m_xVial.SetPosition(rect);
}

void Hud::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xVial.Update(frame);
	m_xBuffs.Update(frame);
	m_xLives.Update(frame);
}

void Hud::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;
	
	if (!m_bIsEnabled) {
		return;
	}

	m_xVial.Render(renderer, frame);
	m_xBuffs.Render(renderer, frame);
	m_xLives.Render(renderer, frame);
}
