#ifndef __HUDBUFFPANEL_H__
#define __HUDBUFFPANEL_H__

#include "HudBuffButton.h"
#include "Window.h"

#include <vector>

class HudBuffPanel : public Window {
private:
	bool m_bIsEnabled;
	std::vector<HudBuffButton*> m_xBuffs;
	
public:
	HudBuffPanel();
	~HudBuffPanel();
	
	void Initialize();
	void SetEnabled(bool enabled);

	void AddBuff(GameFoundation::BuffType bt);
	void ClearBuffs();

protected:
	virtual void OnDoLayout(const CIwSVec2& screensize);
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);

private:
	void BuffTriggerHandler(const HudBuffButton& sender, const HudBuffButton::BuffTriggerArgs& args);
	
public:
	struct BuffTriggerArgs {
		GameFoundation::BuffType bufftype;
		int bufflevel;
	};
	MulticastEvent<HudBuffPanel, BuffTriggerArgs> BuffTrigger;
};

#endif
