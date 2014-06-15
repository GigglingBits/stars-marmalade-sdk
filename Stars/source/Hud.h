#ifndef __HUD_H__
#define __HUD_H__

#include "Window.h"
#include "GameFoundation.h"
#include "ButtonEx.h"
#include "LevelDustVial.h"
#include "HudNumber.h"

class Hud : public Window {
private:
	GameFoundation& m_rxGame;

	bool m_bIsEnabled;

	LevelDustVial m_xVial;
	
	ButtonEx m_xButtonMagnet;
	ButtonEx m_xButtonShield;
	ButtonEx m_xButtonShoot;

public:
	Hud(GameFoundation& game);
	virtual ~Hud();

	virtual void Initialize();
	
	void SetEnabled(bool enabled);
	
protected:
	virtual void OnDoLayout(const CIwSVec2& screensize);
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	
private:
	void SetButtonTexture(ButtonEx& button, const std::string& skin);
	
	void ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args);
	void BuffCountChangedEventHandler(const GameFoundation& sender, const GameFoundation::BuffContainer& args);
};

#endif
