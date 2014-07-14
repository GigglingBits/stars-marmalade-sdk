#ifndef __BUTTONEX_H__
#define __BUTTONEX_H__

#include "Button.h"

#define BUTTONEX_PRESSED "pressed"
#define BUTTONEX_RELEASED "released"
#define BUTTONEX_DISABLED "disabled"

class ButtonEx : public Button {
private:
	std::string m_sDisabled;
	std::string m_sReleased;
	std::string m_sPressed;
	
public:
	ButtonEx(ButtonCommandId cmdid, s3eKey key, long userdata = 0);

	void SetTextureFrameDisabled(const std::string& name);
	void SetTextureFrameReleased(const std::string& name);
	void SetTextureFramePressed(const std::string& name);
	
protected:
	virtual void OnTextureLoaded(Texture& texture);
	virtual void OnStateChanged(bool enabled, bool pressed);
};

#endif
