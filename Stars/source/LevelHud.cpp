#include "LevelHud.h"
#include "Debug.h"
#include "FactoryManager.h"

LevelHud::LevelHud(GameFoundation& game) :
	m_rxGame(game),
	m_xButtonMagnet(eButtonCommandIdStarMagnet, s3eKey1),
	m_xButtonShield(eButtonCommandIdStarShield, s3eKey2),
	m_xButtonShoot(eButtonCommandIdStarShoot, s3eKey3),
	m_bIsEnabled(true) {
	m_xButtonMagnet.PressedEvent.AddListener(this, &LevelHud::ButtonPressedEventHandler);
	m_xButtonShield.PressedEvent.AddListener(this, &LevelHud::ButtonPressedEventHandler);
	m_xButtonShoot.PressedEvent.AddListener(this, &LevelHud::ButtonPressedEventHandler);
	m_rxGame.BuffCountChanged.AddListener(this, &LevelHud::BuffCountChangedEventHandler);
}

LevelHud::~LevelHud() {
	m_rxGame.BuffCountChanged.RemoveListener(this, &LevelHud::BuffCountChangedEventHandler);
	m_xButtonMagnet.PressedEvent.RemoveListener(this, &LevelHud::ButtonPressedEventHandler);
	m_xButtonShield.PressedEvent.RemoveListener(this, &LevelHud::ButtonPressedEventHandler);
	m_xButtonShoot.PressedEvent.RemoveListener(this, &LevelHud::ButtonPressedEventHandler);
}

void LevelHud::Initialize() {
	m_xButtonMagnet.SetTexture(FactoryManager::GetTextureFactory().Create("button_action_block"));
	m_xButtonShield.SetTexture(FactoryManager::GetTextureFactory().Create("button_action_block"));
	m_xButtonShoot.SetTexture(FactoryManager::GetTextureFactory().Create("button_action_attack"));
	
	m_xVial.Initialize();
}

void LevelHud::SetEnabled(bool enabled) {
	if (m_bIsEnabled != enabled) {
		m_xButtonMagnet.SetEnabled(enabled);
		m_xButtonShield.SetEnabled(enabled);
		m_xButtonShoot.SetEnabled(enabled);
	}
	
	m_bIsEnabled = enabled;
}

void LevelHud::OnDoLayout(const CIwSVec2& screensize) {
	int extent = GetScreenExtents();
	
	int spacing = extent / 40;
	
	int buttonwidth = extent / 8;
	int buttonheight = extent / 8;

	int dustvialheight = extent / 3;
	int dustvialwidth = extent / 7;
	
	// action buttons (right)
	CIwRect rect;
	rect.x = spacing;
	rect.y = screensize.y - (3 * buttonheight) - (3 * spacing);
	rect.w = buttonwidth;
	rect.h = buttonheight;
	m_xButtonMagnet.SetPosition(rect);
	rect.y += rect.h + spacing;
	m_xButtonShield.SetPosition(rect);
	rect.y += rect.h + spacing;
	m_xButtonShoot.SetPosition(rect);
	
	// dust vial
	int x = screensize.x - dustvialwidth - spacing;
	int y = screensize.y - dustvialheight - spacing;
	int w = dustvialwidth;
	int h = dustvialheight;
	rect.Make(x, y, w, h);
	m_xVial.SetPosition(rect);
}

void LevelHud::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xButtonMagnet.Update(frame);
	m_xButtonShield.Update(frame);
	m_xButtonShoot.Update(frame);
	
	m_xVial.SetDustAmount(m_rxGame.GetDustFillPercent(), m_rxGame.GetDustQueuedPercent());
	m_xVial.Update(frame);
}

void LevelHud::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (!m_bIsEnabled) {
		return;
	}
	
	m_xButtonMagnet.Render(renderer, frame);
	m_xButtonShield.Render(renderer, frame);
	m_xButtonShoot.Render(renderer, frame);
	
	m_xVial.Render(renderer, frame);
}

void LevelHud::ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args) {
	switch (args.id) {
		case eButtonCommandIdStarMagnet:
			m_rxGame.ActivateMagnetBuff();
			break;
		case eButtonCommandIdStarShield:
			m_rxGame.ActivateShieldBuff();
			break;
		case eButtonCommandIdStarShoot:
			m_rxGame.ActivateShootBuff();
			break;
		default:
			break;
	}
}

void LevelHud::BuffCountChangedEventHandler(const GameFoundation& sender, const GameFoundation::BuffContainer& args) {
}
