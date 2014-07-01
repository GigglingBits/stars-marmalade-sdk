#ifndef __HUDBUFFBUTTON_H__
#define __HUDBUFFBUTTON_H__

#include "Button.h"
#include "GameFoundation.h"

class HudBuffButton : public Button {
private:
	bool m_bIsUsed;
	
public:
	HudBuffButton(GameFoundation::BuffType bt);
	~HudBuffButton();
	
	bool CanUnload();
	
protected:
	virtual void OnTextureLoaded(Texture& texture);
	virtual void OnStateChanged(bool enabled, bool pressed);
	
private:
	GameFoundation::BuffType GetBuffType();
	std::string GetSkinName(GameFoundation::BuffType bt);

public:
	MulticastEvent<HudBuffButton, GameFoundation::BuffType> BuffTrigger;
};

#endif
