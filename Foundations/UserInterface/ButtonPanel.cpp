#include "ButtonPanel.h"
#include "SoundEngine.h"

#include "Debug.h"

ButtonPanel::ButtonPanel(ButtonCommandId cmdid, s3eKey key, long userdata)
	: m_xToggle(cmdid, key, userdata), m_xCurtain(0xdd, 100), m_bEnabled(true), m_bPanelVisible(false), m_iButtonCount(0) {

	m_xToggle.PressedEvent.AddListener<ButtonPanel>(this, &ButtonPanel::PressedEventHandler);

	for (int i = 0; i < BTNPANEL_MAX_BTN_COUNT; i++) {
		m_apxButtons[i] = NULL;
	}

	SetButtonsEnabled(false);
}

ButtonPanel::~ButtonPanel() {
	m_xToggle.PressedEvent.RemoveListener<ButtonPanel>(this, &ButtonPanel::PressedEventHandler);
}

void ButtonPanel::Initialize() {
}

bool ButtonPanel::IsPanelVisible() {
	return m_bPanelVisible;
}

bool ButtonPanel::IsEnabled() {
	return m_bEnabled;
}

void ButtonPanel::SetEnabled(bool enabled) {
	m_bEnabled = enabled;
	m_xToggle.SetEnabled(enabled);
	SetButtonsEnabled(IsPanelVisible() && m_bEnabled);
}

void ButtonPanel::SetButtonsEnabled(bool enabled) {
	for (int i = 0; i < m_iButtonCount; i++) {
		if (m_apxButtons[i]) {
			m_apxButtons[i]->SetEnabled(enabled && m_bEnabled);
		}
	}
}

Button& ButtonPanel::GetMainButton() {
	return m_xToggle;
}

void ButtonPanel::AddButton(Button& button) {
	IW_CALLSTACK_SELF;

	IwAssertMsg(MYAPP, m_iButtonCount < BTNPANEL_MAX_BTN_COUNT, ("Cannot add button; the button array is full."));

	button.SetEnabled(m_bPanelVisible);
	m_apxButtons[m_iButtonCount] = &button;
	m_iButtonCount++;
}

void ButtonPanel::ShowPanel() {
	if (m_bPanelVisible && m_xCurtain.IsClosed()) {
		// already shown
		return;
	}

	if (!m_bPanelVisible && m_xCurtain.IsOpen()) {
		// show
		m_xCurtain.Close();
	}
}

void ButtonPanel::OnDoLayout(const CIwSVec2& screensize) {
	int extents = GetScreenExtents();
	int width = extents / 7;	 
	int space = width / 2;

	CIwRect rect(0, 0, width, width);
	for (int i = 0; i < m_iButtonCount; i++) {
		if (Button* p = m_apxButtons[i]) {
			rect.x = (screensize.x - (m_iButtonCount * (width + space) - space)) / 2 + (i * (width + space));
			rect.y = (screensize.y - width) / 2;
			p->SetPosition(rect);
		}
	}
}

void ButtonPanel::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xCurtain.Update(frame);
	m_xToggle.Update(frame);

	for (int i = 0; i < m_iButtonCount; i++) {
		if (m_apxButtons[i]) {
			m_apxButtons[i]->Update(frame);
		}
	}

	if (!m_xCurtain.IsClosed() && m_bPanelVisible) {
		m_bPanelVisible = false;
		SetButtonsEnabled(false);
	} else if (m_xCurtain.IsClosed() && !m_bPanelVisible) {
		m_bPanelVisible = true;
		SetButtonsEnabled(true);
	}
}

void ButtonPanel::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xCurtain.Render(renderer, frame);
	m_xToggle.Render(renderer, frame);

	for (int i = 0; i < m_iButtonCount; i++) {
		if (m_apxButtons[i]) {
			m_apxButtons[i]->Render(renderer, frame);
		}
	}
}

void ButtonPanel::PressedEventHandler(const Button& sender, const Button::EventArgs& args) {
	if (!m_bEnabled && m_xCurtain.IsOpen()) {
		return;
	}

	bool handled = false;
	bool open = false;
	if (m_xCurtain.IsOpen()) {
		handled = true;
		m_xCurtain.Close();
		open = false;
	} else if (m_xCurtain.IsClosed()) {
		handled = true;
		m_xCurtain.Open();
		open = true;
	}

	if (handled) {
		// inform the sender that the event was handled
		Button::EventArgs* pargs = (Button::EventArgs*)&args;
		pargs->handled = true;

		// fire event about state change
		EventArgs panelargs;
		panelargs.IsOpen = open;
		StateChanged(*this, panelargs); 
	}
}
