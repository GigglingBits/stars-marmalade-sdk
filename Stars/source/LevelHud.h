#ifndef __LEVELHUD_H__
#define __LEVELHUD_H__

#include "Window.h"
#include "GameFoundation.h"
#include "ButtonEx.h"
#include "LevelDustCollector.h"
#include "LevelHudNumber.h"

class LevelHud : public Window {
private:
	GameFoundation& m_rxGame;

	ButtonEx m_xButtonBlock;
	ButtonEx m_xButtonAttack;
	
	LevelDustCollector m_xDustCollector;

	LevelHudNumber m_xQueuedCount;
	LevelHudNumber m_xQueuedAmount;
	
public:
	LevelHud(GameFoundation& game);
	virtual ~LevelHud();

	virtual void Initialize();
	
protected:
	virtual void OnDoLayout(const CIwSVec2& screensize);
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	
private:
	void ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args);
	void ButtonReleasedEventHandler(const Button& sender, const Button::EventArgs& args);
};

#endif
