#ifndef __APPPANEL_H__
#define __APPPANEL_H__

#include "ButtonPanel.h"
#include "Button.h"

class AppPanel : public ButtonPanel {
private:
	Button m_xBtnQuit;
	Button m_xBtnRestart;
	Button m_xBtnReload;
	Button m_xBtnReloadLocal;

public:
	AppPanel(ButtonCommandId cmdid, s3eKey key, long userdata = 0);
	virtual void Initialize();
};

#endif
