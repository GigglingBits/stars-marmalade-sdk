#include "NaviPanel.h"
#include "FactoryManager.h"
#include "Debug.h"

NaviPanel::NaviPanel() : m_iButtonCount(0) {
	memset(m_apxButtons, 0, sizeof(m_apxButtons));
}

NaviPanel::~NaviPanel() {
	for (int i = 0; i < m_iButtonCount; i++) {
		IwAssert(MYAPP, m_apxButtons[i]);
		delete m_apxButtons[i];
	}
}

void NaviPanel::Initialize() {
}

void NaviPanel::OnDoLayout(const CIwSVec2& screensize) {
	if (m_iButtonCount < 1) {
		return;
	}
	
	// set button geometry
	CIwRect rect(GetPosition());
	rect.w /= m_iButtonCount;
	for (int i = 0; i < m_iButtonCount; i++) {
		m_apxButtons[i]->SetPosition(rect);
		rect.x += rect.w;
	}
}

void NaviPanel::OnUpdate(const FrameData& frame) {
	for (int i = 0; i < m_iButtonCount; i++) {
		m_apxButtons[i]->Update(frame);
	}
}

void NaviPanel::OnRender(Renderer& renderer, const FrameData& frame) {
	for (int i = 0; i < m_iButtonCount; i++) {
		m_apxButtons[i]->Render(renderer, frame);
	}
}

void NaviPanel::AddButton(const std::string& textureid, int buttonid) {
	IW_CALLSTACK_SELF;
	
	// find empty spot
	int cursor = m_iButtonCount;
	if (cursor == NAVIPANEL_BUTTON_MAX_COUNT - 1) {
		IwAssertMsg(MYAPP, false, ("Cannot add button. Maximum number of buttons exceeded."));
		return;
	}

	// load and check the texture
	Texture* t = FactoryManager::GetTextureFactory().Create(textureid);
	if (t) {
		if (!(t->ContainsFrame(NAVIPANEL_BUTTON_ON) && t->ContainsFrame(NAVIPANEL_BUTTON_OFF))) {
			IwAssertMsg(MYAPP, false, ("The texture '%s' cannot be used in the NaviBar. It does not contain frames called 'on' and 'off'.", textureid.c_str()));
			delete t;
			return;
		}
	} else {
		return;
	}
	
	// create the button
	m_apxButtons[cursor] = new Button(eButtonCommandIdNone, s3eKeyFirst, buttonid);
	m_apxButtons[cursor]->SetTexture(t);
	m_apxButtons[cursor]->SetHideWhenDisabled(true);
	m_apxButtons[cursor]->SetShadedWhenPressed(false);

	// houskeeping
	m_iButtonCount++;
	InvalidateLayout();
}

void NaviPanel::ActivateButton(int buttonid) {
	for (int i = 0; i < m_iButtonCount; i++) {
		if (m_apxButtons[i]->GetUserData() == buttonid) {
			m_apxButtons[i]->SetTextureFrame(NAVIPANEL_BUTTON_ON);
		} else {
			m_apxButtons[i]->SetTextureFrame(NAVIPANEL_BUTTON_OFF);
		}
	}
}
