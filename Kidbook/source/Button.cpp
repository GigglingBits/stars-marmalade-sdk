#include <string>

#include "Button.h"

Button::Button(uint8 id) {
	m_bEnabled = true;
	m_iId = id;

	std::string basename;
	switch (id) {
	case BTN_ID_SPEECH:
		basename = "buttons/speech";
		break;
	case BTN_ID_SOUND:
		basename = "buttons/sound";
		break;
	case BTN_ID_TEXT:
		basename = "buttons/text";
		break;
	case BTN_ID_EXIT:
		basename = "buttons/exit";
		break;
	case BTN_ID_HELP:
		basename = "buttons/help";
		break;
	}

	if (!(m_pxFaceEnabled = Iw2DCreateImage((std::string(basename).append(".png")).c_str()))) {
		IwAssertMsg(MYAPP, false, ("ERROR: Could not load button image: %s", basename.c_str()));
	}
	if (!(m_pxFaceDisabled = Iw2DCreateImage((std::string(basename).append("_disabled.png")).c_str()))) {
		IwAssertMsg(MYAPP, false, ("ERROR: Could not load disabled button image: %s", basename.c_str()));
	}
}

Button::~Button() {
	if (m_pxFaceEnabled) {
		delete m_pxFaceEnabled;
	}
	if (m_pxFaceDisabled) {
		delete m_pxFaceDisabled;
	}
}

uint8 Button::GetId() {
	return m_iId;
}

void Button::Enable(bool enable) {
	m_bEnabled = enable;
}

bool Button::HitTest(CIwSVec2 pos) {
	return 
		m_bEnabled &&
		m_xFaceRegion.x <= pos.x &&
		m_xFaceRegion.y <= pos.y &&
		m_xFaceRegion.x + m_xFaceRegion.w >= pos.x &&
		m_xFaceRegion.y + m_xFaceRegion.h >= pos.y;
}

bool Button::OnUpdate(bool first) {
	return false;
}

void Button::OnRender(const CIwSVec2& screensize, const CIwRect& itemregion) {
	CIw2DImage* pImage = m_bEnabled ? m_pxFaceEnabled : m_pxFaceDisabled;
	if (pImage) {
		Iw2DSetColour(0xffffffff);
		Iw2DDrawImage(pImage, CIwFVec2(itemregion.x, itemregion.y), CIwFVec2(itemregion.w, itemregion.h));
	}
	m_xFaceRegion = itemregion;
}
