#ifndef __SOCIALPANEL_H__
#define __SOCIALPANEL_H__

#include "ButtonPanel.h"
#include "Timer.h"

class SocialPanel : public ButtonPanel {
private:
	Button m_xCredits;
	Button m_xAchievements;

public:
	SocialPanel(ButtonCommandId cmdid, s3eKey key, long userdata = 0);

	virtual void Initialize();
};

#endif
