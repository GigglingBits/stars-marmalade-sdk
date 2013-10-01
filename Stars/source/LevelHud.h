#ifndef __LEVELHUD_H__
#define __LEVELHUD_H__

#include "Window.h"
#include "GameFoundation.h"
#include "ButtonEx.h"
#include "LevelDustCollector.h"
#include "LevelHudNumber.h"
#include "LevelProgressBar.h"

class LevelHud : public Window {
private:
	GameFoundation& m_rxGame;

	bool m_bIsEnabled;

	LevelProgressBar m_xProgressBar;

	VertexStreamScreen m_xVialShape;
	Texture* m_pxVial;

	LevelDustCollector m_xDustQueue;
	LevelDustCollector m_xDustCollector;

	LevelHudNumber m_xQueuedAmount;
	LevelHudNumber m_xCollectedAmount;

	ButtonEx m_xButtonBlock;
	ButtonEx m_xButtonAttack;

public:
	LevelHud(GameFoundation& game);
	virtual ~LevelHud();

	virtual void Initialize();
	
	void SetEnabled(bool enabled);
	
protected:
	virtual void OnDoLayout(const CIwSVec2& screensize);
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	
private:
	void ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args);
	void ButtonReleasedEventHandler(const Button& sender, const Button::EventArgs& args);
};

#endif
