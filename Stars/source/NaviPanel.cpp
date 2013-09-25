#include "NaviPanel.h"
#include "FactoryManager.h"
#include "Debug.h"

NaviPanel::NaviPanel() : m_iButtonCount(0), m_xPosition(0, 0, 0, 0) {
	for (int i = 0; i < NAVIPANEL_MAX_BUTTONS; i++) {
		m_apxButtons[i] = NULL;
	}
}

NaviPanel::~NaviPanel() {
	for (int i = 0; i < m_iButtonCount; i++) {
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
	CIwRect rect(m_xPosition);
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

void NaviPanel::SetPosition(const CIwRect& rect) {
	m_xPosition = rect;
	InvalidateLayout();
}

void NaviPanel::AddButton(const std::string& textureid, ButtonCommandId cmdid, s3eKey key, long userdata) {
	IW_CALLSTACK_SELF;
	
	// find empty spot
	int cursor = m_iButtonCount;
	if (cursor == NAVIPANEL_MAX_BUTTONS - 1) {
		IwAssertMsg(MYAPP, false, ("Cannot add button. Maximum number of buttons exceeded."));
		return;
	}

	// load and check the texture
	Texture* t = FactoryManager::GetTextureFactory().Create(textureid);
	if (t) {
		if (!(t->ContainsFrame("on") && t->ContainsFrame("on"))) {
			IwAssertMsg(MYAPP, false, ("The texture '%s' cannot be used in the NaviBar. It does not contain frames called 'on' and 'off'.", textureid.c_str()));
			delete t;
			return;
		}
	} else {
		return;
	}
	
	// create the button
	m_apxButtons[cursor] = new Button(cmdid, key, userdata);
	m_apxButtons[cursor]->SetTexture(t);

	// houskeeping
	m_iButtonCount++;
	InvalidateLayout();
}
