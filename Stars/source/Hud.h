#ifndef __HUD_H__
#define __HUD_H__

#include "Window.h"
#include "GameFoundation.h"
#include "LevelDustVial.h"
#include "HudNumber.h"
#include "HudBuffPanel.h"

class Hud : public Window {
private:
	GameFoundation& m_rxGame;

	bool m_bIsEnabled;
	
	LevelDustVial m_xVial;
	HudBuffPanel m_xBuffs;

public:
	Hud(GameFoundation& game);

	virtual void Initialize();
	void SetEnabled(bool enabled);
	
protected:
	virtual void OnDoLayout(const CIwSVec2& screensize);
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
