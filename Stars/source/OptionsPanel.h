#ifndef __OPTIONSPANEL_H__
#define __OPTIONSPANEL_H__

#include "ButtonPanel.h"
#include "Timer.h"

class OptionsPanel : public ButtonPanel {
private:
	Button m_xSoundFx;
	Button m_xMusic;
	Button m_xReload;
	Button m_xReloadLocal;

	Timer m_xDemoTimer;

public:
	OptionsPanel(ButtonCommandId cmdid, s3eKey key, long userdata = 0);
	virtual ~OptionsPanel();
	virtual void Initialize();

protected:
	virtual void OnUpdate(const FrameData& frame);

private:
	void DemoTimerElapsedEventHandler(const Timer& sender, const long& data);
};

#endif
