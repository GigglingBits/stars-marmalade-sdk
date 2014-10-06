#ifndef __LEVELADSPANEL_H__
#define __LEVELADSPANEL_H__

#include "ButtonPanel.h"
#include "Button.h"

class LevelAdsPanel : public Window {
private:
	Button m_xBtnWatchAd;
	Button m_xBtnRemoveAds;

public:
	LevelAdsPanel();
	virtual void Initialize();
};

#endif
