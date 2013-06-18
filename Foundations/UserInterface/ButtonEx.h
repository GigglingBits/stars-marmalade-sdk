#ifndef __BUTTONEX_H__
#define __BUTTONEX_H__

#include "Button.h"

#define BUTTONEX_PRESSED "pressed"
#define BUTTONEX_RELEASED "released"
#define BUTTONEX_DISABLED "disabled"

class ButtonEx : public Button {
public:
	ButtonEx(ButtonCommandId cmdid, s3eKey key, long userdata = 0);

protected:
	virtual void OnTextureLoaded(Texture& texture);
	virtual void OnStateChanged(bool enabled, bool pressed);
};

#endif
