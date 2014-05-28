#ifndef __LEVELPAUSEDPANEL_H__
#define __LEVELPAUSEDPANEL_H__

#include "ButtonPanel.h"
#include "Button.h"

class LevelPausedPanel : public ButtonPanel {
private:
	Button m_xBtnQuit;
	Button m_xBtnRestart;
	Button m_xBtnReload;
	Button m_xBtnReloadLocal;

public:
	LevelPausedPanel(ButtonCommandId cmdid, s3eKey key, long userdata = 0);
	virtual void Initialize();
	
protected:
	virtual void OnDoLayout(const CIwSVec2& screensize);
};

#endif
