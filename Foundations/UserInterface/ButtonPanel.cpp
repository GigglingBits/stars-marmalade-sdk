#include "ButtonPanel.h"
#include "SoundEngine.h"

#include "Debug.h"

ButtonPanel::ButtonPanel(ButtonCommandId cmdid, s3eKey key, long userdata)
	: m_xToggle(cmdid, key, userdata), m_xCurtain(0xdd, 100), m_bPanelEnabled(true), m_bPanelOpen(false), m_iButtonCount(0) {

	m_xCurtain.SetRenderingLayer(Renderer::eRenderingLayerHud3);
		
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

void ButtonPanel::OpenPanel() {
	if (m_bPanelOpen && m_xCurtain.IsClosed()) {
		// already open
		return;
	}
	
	if (!m_bPanelOpen && m_xCurtain.IsOpen()) {
		// show
		m_xCurtain.Close();
	}
}

bool ButtonPanel::IsPanelOpen() {
	return m_bPanelOpen;
}

void ButtonPanel::SetAlpha(uint32 alpha) {
	m_xCurtain.SetAlpha(alpha);
}

void ButtonPanel::SetFadeTime(uint32 fadetime) {
	m_xCurtain.SetFadeTime(fadetime);
}

bool ButtonPanel::IsEnabled() {
	return m_bPanelEnabled;
}

void ButtonPanel::SetEnabled(bool enabled) {
	m_bPanelEnabled = enabled;
	m_xToggle.SetEnabled(enabled);
	SetButtonsEnabled(IsPanelOpen() && m_bPanelEnabled);
}

void ButtonPanel::SetButtonsEnabled(bool enabled) {
	for (int i = 0; i < m_iButtonCount; i++) {
		if (m_apxButtons[i]) {
			m_apxButtons[i]->SetEnabled(enabled && m_bPanelEnabled);
		}
	}
}

Button& ButtonPanel::GetMainButton() {
	return m_xToggle;
}

void ButtonPanel::AddButton(Button& button) {
	IW_CALLSTACK_SELF;

	IwAssertMsg(MYAPP, m_iButtonCount < BTNPANEL_MAX_BTN_COUNT, ("Cannot add button; the button array is full."));

	button.SetEnabled(m_bPanelOpen);
	m_apxButtons[m_iButtonCount] = &button;
	m_iButtonCount++;
}

int ButtonPanel::GetButtonCount() {
	return m_iButtonCount;
}

Button* ButtonPanel::GetButton(int position) {
	if (position >= m_iButtonCount || position < 0) {
		return NULL;
	}
	return m_apxButtons[position];
}

void ButtonPanel::OnDoLayout(const CIwSVec2& screensize) {
	int extents = GetScreenExtents();
	int width = extents / 6;
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

	if (!m_xCurtain.IsClosed() && m_bPanelOpen) {
		m_bPanelOpen = false;
		SetButtonsEnabled(false);
	} else if (m_xCurtain.IsClosed() && !m_bPanelOpen) {
		m_bPanelOpen = true;
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
	if (!m_bPanelEnabled && m_xCurtain.IsOpen()) {
		return;
	}

	bool handled = false;
	bool curtainopen = false;
	if (m_xCurtain.IsOpen()) {
		handled = true;
		m_xCurtain.Close();
		curtainopen = false;
	} else if (m_xCurtain.IsClosed()) {
		handled = true;
		m_xCurtain.Open();
		curtainopen = true;
	}

	if (handled) {
		// inform the sender that the event was handled
		Button::EventArgs* pargs = (Button::EventArgs*)&args;
		pargs->handled = true;

		// fire event about state change
		EventArgs panelargs;
		panelargs.IsPanelOpen = !curtainopen;
		StateChanged(*this, panelargs); 
	}
}
