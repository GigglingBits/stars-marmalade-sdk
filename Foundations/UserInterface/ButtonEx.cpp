#include "ButtonEx.h"

#include "Debug.h"

ButtonEx::ButtonEx(ButtonCommandId cmdid, s3eKey key, long userdata)
	: Button(cmdid, key, userdata), m_sDisabled(BUTTONEX_DISABLED), m_sReleased(BUTTONEX_RELEASED), m_sPressed(BUTTONEX_PRESSED) {
	SetHideWhenDisabled(false);
}

void ButtonEx::SetTextureFrameDisabled(const std::string& name) {
	IW_CALLSTACK_SELF;
	m_sDisabled = name;
	IwAssertMsg(MYAPP, !IsTextureLoaded(), ("Texture is already loaded. Please set this name before loading the texture."));
	OnStateChanged(IsEnabled(), IsPressed());
}

void ButtonEx::SetTextureFrameReleased(const std::string& name) {
	IW_CALLSTACK_SELF;
	m_sReleased = name;
	IwAssertMsg(MYAPP, !IsTextureLoaded(), ("Texture is already loaded. Please set this name before loading the texture."));
	OnStateChanged(IsEnabled(), IsPressed());
}

void ButtonEx::SetTextureFramePressed(const std::string& name) {
	IW_CALLSTACK_SELF;
	m_sPressed = name;
	IwAssertMsg(MYAPP, !IsTextureLoaded(), ("Texture is already loaded. Please set this name before loading the texture."));
	OnStateChanged(IsEnabled(), IsPressed());
}

void ButtonEx::OnTextureLoaded(Texture& texture) {
	IW_CALLSTACK_SELF;
	
	IwAssertMsg(MYAPP,
		texture.ContainsFrame(BUTTONEX_PRESSED),
		("Texture is used in a button. But the frame %s cannot be found.", m_sPressed.c_str()));
	
	IwAssertMsg(MYAPP,
		texture.ContainsFrame(BUTTONEX_RELEASED),
		("Texture is used in a button. But the frame %s cannot be found.", m_sReleased.c_str()));

	IwAssertMsg(MYAPP,
		texture.ContainsFrame(BUTTONEX_DISABLED),
		("Texture is used in a button. But the frame %s cannot be found.", m_sDisabled.c_str()));

	OnStateChanged(IsEnabled(), IsPressed());
}

void ButtonEx::OnStateChanged(bool enabled, bool pressed) {
	IW_CALLSTACK_SELF;
	if (!IsTextureLoaded()) {
		return;
	}
	
	if (!enabled && m_sDisabled.length() > 0) {
		SetTextureFrame(m_sDisabled);
	} else if (pressed  && m_sPressed.length() > 0) {
		SetTextureFrame(m_sPressed);
	} else if (m_sReleased.length() > 0) {
		SetTextureFrame(m_sReleased);
	} else {
		IwAssertMsg(MYAPP, false, ("No suitable texture frame found to represent the button state."));
	}
}
