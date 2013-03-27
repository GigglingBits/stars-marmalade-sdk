#include "Button.h"
#include "SoundEngine.h"

#include "Debug.h"

Button::Button(ButtonCommandId cmdid, s3eKey key, long userdata)
	: m_pxTexture(NULL), m_bDown(false), m_bHideWhenDisabled(true) {

	m_xButton.cmdid = cmdid;
	m_xButton.key = key;
	m_xButton.userdata = userdata;

	m_xButtonVerts.SetRect(m_xButton.rect);

	m_xButton.PressedEvent.AddListener<Button>(this, &Button::PressedEventHandler);
	m_xButton.ReleasedEvent.AddListener<Button>(this, &Button::ReleasedEventHandler);
}

Button::~Button() {
	if (m_pxTexture) {
		delete m_pxTexture;
	}

	m_xButton.PressedEvent.RemoveListener<Button>(this, &Button::PressedEventHandler);
	m_xButton.ReleasedEvent.RemoveListener<Button>(this, &Button::ReleasedEventHandler);
}

void Button::SetUserData(long userdata) {
	m_xButton.userdata = userdata;
}

void Button::SetTexture(Texture* texture) {
	if (!texture) {
		return;
	}

	if (m_pxTexture) {
		delete m_pxTexture;
	}
	m_pxTexture = texture;
}

void Button::SetTextureFrame(const std::string& frame) {
	if (m_pxTexture) {
		if (!m_pxTexture->SelectFrame(frame)) {
			IwAssertMsg(MYAPP, false, ("Could not select frame %s in current button texture.", frame.c_str()));
		}
	} else {
		IwAssertMsg(MYAPP, false, ("Cannot set texture frame, since no texture is loaded."));
	}
}

void Button::SetText(const std::string& text, uint32 colour) {
	m_sText = text;
	m_uiTextCol = colour;
}

void Button::SetEnabled(bool enabled) {
	m_xButton.enabled = enabled;
}

void Button::SetHideWhenDisabled(bool hide) {
	m_bHideWhenDisabled = hide;
}

void Button::SetPosition(const CIwRect& rect) {
	m_xButton.rect = rect;
	m_xButtonVerts.SetRect(rect);
}

const CIwRect& Button::GetPosition() {
	return m_xButton.rect;
}

void Button::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (m_pxTexture) {
		m_pxTexture->Update(frame.GetRealDurationMs());
	}
}

void Button::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (!m_xButton.enabled && m_bHideWhenDisabled) {
		return;
	}

	if (m_pxTexture) {
		renderer.Draw(m_xButtonVerts, *m_pxTexture); 
	}

	if (!m_sText.empty()) {
		renderer.DrawText(m_sText, m_xButton.rect, Renderer::eFontTypeNormal, m_uiTextCol); 
	}

	if (m_bDown) {
		renderer.DrawRect(m_xButton.rect, 0x00000000, 0x77000000);
	}
}

void Button::PressedEventHandler(const InputManager::VirtualButton& sender, const InputManager::VirtualButton::EventArgs& args) {
	IwAssertMsg(MYAPP, m_xButton.enabled, ("Did not expect this handler to be called when the button was disabled!"));

	// RRR: de-const
	InputManager::VirtualButton::EventArgs* pargs = (InputManager::VirtualButton::EventArgs*)&args;

	if (m_xButton.enabled) {
		EventArgs newargs;
		newargs.handled = false;
		PressedEvent.Invoke(*this, newargs);
		pargs->handled = newargs.handled;

		SoundEngine::PlaySoundEffect("ButtonPressed");
		m_bDown = true;
	} else {
		// button is disabled; suppress any further processing
		pargs->handled = true;
		m_bDown = false;
	}
}

void Button::ReleasedEventHandler(const InputManager::VirtualButton& sender, const InputManager::VirtualButton::EventArgs& args) {
	// RRR: de-const
	InputManager::VirtualButton::EventArgs* pargs = (InputManager::VirtualButton::EventArgs*)&args;

	if (m_xButton.enabled || m_bDown) {
		EventArgs newargs;
		newargs.handled = false;
		ReleasedEvent.Invoke(*this, newargs);
		pargs->handled = newargs.handled;
	} else {
		// button is disabled; suppress any further processing
		pargs->handled = true;
	}
	m_bDown = false;
}
