#ifndef __LEVELPAUSEDPANEL_H__
#define __LEVELPAUSEDPANEL_H__

#include "ButtonPanel.h"
#include "Button.h"

class LevelPausedPanel : public ButtonPanel {
private:
	Button m_xBtnMenu;
	Button m_xBtnRetry;
	Button m_xBtnReload;
	Button m_xBtnReloadLocal;

public:
	LevelPausedPanel(ButtonCommandId cmdid, s3eKey key, long userdata = 0);
	virtual void Initialize();
};

#endif
