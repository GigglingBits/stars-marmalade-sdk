#ifndef __HUDBUFFBUTTON_H__
#define __HUDBUFFBUTTON_H__

#include "Button.h"
#include "GameFoundation.h"

class HudBuffButton : public Button {
private:
	bool m_bIsUsed;
	int m_iBuffLevel;
	std::string m_sBuffLevel;
	
public:
	HudBuffButton(GameFoundation::BuffType bt);
	~HudBuffButton();
	
	bool CanUnload();
	void IncrementLevel();
	
protected:
	virtual void OnTextureLoaded(Texture& texture);
	virtual void OnStateChanged(bool enabled, bool pressed);
	
	virtual void OnRender(Renderer& renderer, const FrameData& frame);

private:
	GameFoundation::BuffType GetBuffType();
	std::string GetSkinName(GameFoundation::BuffType bt);

public:
	struct BuffTriggerArgs {
		GameFoundation::BuffType bufftype;
		int bufflevel;
	};
	MulticastEvent<HudBuffButton, BuffTriggerArgs> BuffTrigger;
};

#endif
