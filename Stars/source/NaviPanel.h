#ifndef __NAVIPANEL_H__
#define __NAVIPANEL_H__

#include "ButtonPanel.h"

class NaviPanel : public ButtonPanel {
private:
	Button m_xSoundFx;
	Button m_xMusic;
	Button m_xReload;
	Button m_xReloadLocal;

public:
	NaviPanel(ButtonCommandId cmdid, s3eKey key, long userdata = 0);
	virtual ~NaviPanel();
	virtual void Initialize();

protected:
	virtual void OnUpdate(const FrameData& frame);
};

#endif
