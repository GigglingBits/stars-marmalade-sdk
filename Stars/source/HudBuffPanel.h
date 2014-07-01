#ifndef __HUDBUFFPANEL_H__
#define __HUDBUFFPANEL_H__

#include "HudBuffButton.h"
#include "GameFoundation.h"
#include "Window.h"

#include <vector>

class HudBuffPanel : public Window {
private:
	GameFoundation& m_rxGame;
	bool m_bIsEnabled;
	std::vector<HudBuffButton*> m_xBuffs;
	
public:
	HudBuffPanel(GameFoundation& game);
	~HudBuffPanel();
	
	void Initialize();
	void SetEnabled(bool enabled);

	void ClearBuffs();

protected:
	virtual void OnDoLayout(const CIwSVec2& screensize);
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
		
private:
	void AddBuff(GameFoundation::BuffType bt);
		
	void BuffCollected(const GameFoundation& sender, const GameFoundation::BuffType& bt);
};

#endif
