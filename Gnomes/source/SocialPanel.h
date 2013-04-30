#ifndef __SOCIALPANEL_H__
#define __SOCIALPANEL_H__

#include "ButtonPanel.h"

class SocialPanel : public ButtonPanel {
private:
	Button m_xAchievements;
	Button m_xLeaderboards;

public:
	SocialPanel(ButtonCommandId cmdid, s3eKey key, long userdata = 0);
	virtual void Initialize();
};

#endif
