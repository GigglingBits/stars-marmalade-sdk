#include "ButtonEx.h"

#include "Debug.h"

ButtonEx::ButtonEx(ButtonCommandId cmdid, s3eKey key, long userdata)
	: Button(cmdid, key, userdata) {
	SetHideWhenDisabled(false);
	SetShadedWhenPressed(false);
}

void ButtonEx::OnTextureLoaded(Texture& texture) {
	IW_CALLSTACK_SELF;
	
	IwAssertMsg(MYAPP,
		texture.ContainsFrame(BUTTONEX_PRESSED),
		("Texture is used in a button. But the frame %s cannot be found.", BUTTONEX_PRESSED));
	
	IwAssertMsg(MYAPP,
		texture.ContainsFrame(BUTTONEX_RELEASED),
		("Texture is used in a button. But the frame %s cannot be found.", BUTTONEX_RELEASED));

	IwAssertMsg(MYAPP,
		texture.ContainsFrame(BUTTONEX_DISABLED),
		("Texture is used in a button. But the frame %s cannot be found.", BUTTONEX_DISABLED));

	OnStateChanged(IsEnabled(), IsPressed());
}

void ButtonEx::OnStateChanged(bool enabled, bool pressed) {
	IW_CALLSTACK_SELF;
	if (!IsTextureLoaded()) {
		return;
	}
	
	if (!enabled) {
		SetTextureFrame(BUTTONEX_DISABLED);
	} else if (pressed) {
		SetTextureFrame(BUTTONEX_PRESSED);
	} else {
		SetTextureFrame(BUTTONEX_RELEASED);
	}
}
