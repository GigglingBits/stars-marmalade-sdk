#include "Hud.h"
#include "Debug.h"
#include "FactoryManager.h"

Hud::Hud(GameFoundation& game) :
	m_rxGame(game),
	m_xButtonMagnet("magnet", eButtonCommandIdStarMagnet, s3eKey1),
	m_xButtonShield("shield", eButtonCommandIdStarShield, s3eKey2),
	m_xButtonShoot("shoot", eButtonCommandIdStarShoot, s3eKey3),
	m_bIsEnabled(true) {
	m_xButtonMagnet.PressedEvent.AddListener(this, &Hud::ButtonPressedEventHandler);
	m_xButtonShield.PressedEvent.AddListener(this, &Hud::ButtonPressedEventHandler);
	m_xButtonShoot.PressedEvent.AddListener(this, &Hud::ButtonPressedEventHandler);
	m_rxGame.BuffCountChanged.AddListener(this, &Hud::BuffCountChangedEventHandler);
}

Hud::~Hud() {
	m_rxGame.BuffCountChanged.RemoveListener(this, &Hud::BuffCountChangedEventHandler);
	m_xButtonMagnet.PressedEvent.RemoveListener(this, &Hud::ButtonPressedEventHandler);
	m_xButtonShield.PressedEvent.RemoveListener(this, &Hud::ButtonPressedEventHandler);
	m_xButtonShoot.PressedEvent.RemoveListener(this, &Hud::ButtonPressedEventHandler);
}

void Hud::Initialize() {
	IW_CALLSTACK_SELF;
	
	m_xButtonMagnet.SetTexture(FactoryManager::GetTextureFactory().Create("buff"));
	m_xButtonShield.SetTexture(FactoryManager::GetTextureFactory().Create("buff"));
	m_xButtonShoot.SetTexture(FactoryManager::GetTextureFactory().Create("buff"));
	
	m_xVial.Initialize();
}

void Hud::SetEnabled(bool enabled) {
	if (m_bIsEnabled != enabled) {
		m_xButtonMagnet.SetEnabled(enabled);
		m_xButtonShield.SetEnabled(enabled);
		m_xButtonShoot.SetEnabled(enabled);
	}
	
	m_bIsEnabled = enabled;
}

void Hud::OnDoLayout(const CIwSVec2& screensize) {
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

void Hud::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xButtonMagnet.Update(frame);
	m_xButtonShield.Update(frame);
	m_xButtonShoot.Update(frame);
	
	m_xVial.SetDustAmount(m_rxGame.GetDustFillPercent(), m_rxGame.GetDustQueuedPercent());
	m_xVial.Update(frame);
}

void Hud::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (!m_bIsEnabled) {
		return;
	}
	
	m_xButtonMagnet.Render(renderer, frame);
	m_xButtonShield.Render(renderer, frame);
	m_xButtonShoot.Render(renderer, frame);
	
	m_xVial.Render(renderer, frame);
}

void Hud::ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args) {
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

void Hud::BuffCountChangedEventHandler(const GameFoundation& sender, const GameFoundation::BuffContainer& args) {
	GameFoundation::BuffContainer::const_iterator it;
	for (it = args.begin(); it != args.end(); it++) {
		switch (it->first) {
			case GameFoundation::eBuffTypeMagnet:
				m_xButtonMagnet.SetCount(it->second);
				break;
			case GameFoundation::eBuffTypeShield:
				m_xButtonShield.SetCount(it->second);
				break;
			case GameFoundation::eBuffTypeShoot:
				m_xButtonShoot.SetCount(it->second);
				break;
			default:
				IwAssertMsg(MYAPP, false, ("Buff not known: %i. It will be ignored.", it->second));
		}
	}
}
