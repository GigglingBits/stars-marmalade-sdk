#include "Iw2D.h"

#include "Toolbar.h"

Toolbar::~Toolbar() {
	Reset();
}

void Toolbar::Reset() {
	for (int i = 0; i < m_xButtons.size(); i++) {
		delete m_xButtons.element_at(i);
	}
	m_xButtons.clear();
}

void Toolbar::AddButton(uint8 id) {
	Button* pButton = new Button(id);
	m_xButtons.append(pButton);
}

int Toolbar::GetButtonCount() {
	return m_xButtons.size();
}

int Toolbar::TouchButton(CIwSVec2 pos) {
	for (int i = 0; i < m_xButtons.size(); i++) {
		Button* pButton = m_xButtons.element_at(i);
		if (pButton->HitTest(pos)) {
			return pButton->GetId();
		}
	}
	return -1;
}

void Toolbar::EnableButton(uint8 id, bool enabled) {
	for (int i = 0; i < m_xButtons.size(); i++) {
		Button* pButton = m_xButtons.element_at(i);
		if (pButton->GetId() == id) {
			pButton->Enable(enabled);
			return;
		}
	}
}

bool Toolbar::OnUpdate(bool first) {
	for (int i = 0; i < m_xButtons.size(); i++) {
		if (m_xButtons.element_at(i)->Update(first)) {
			return true;
		}
	}
	return false;
}

void Toolbar::OnRender(const CIwSVec2& screensize, const CIwRect& itemregion) {
	// calculate button geometry
	const int margin = 15;
	const int count = m_xButtons.size();

	if (count <= 0) {
		return;
	}

	CIwRect buttonrect(
		0, 0,
		((itemregion.w - margin) / count) - margin,
		itemregion.h - (margin * 2));
	buttonrect.w = std::min(buttonrect.w, buttonrect.h); // make square
	buttonrect.h = std::min(buttonrect.w, buttonrect.h);

	// draw buttons
	for (int i = 0; i < m_xButtons.size(); i++) {
		buttonrect.x = itemregion.x + (i * (buttonrect.w + margin)) + margin;
		buttonrect.y = itemregion.y + margin;
		m_xButtons.element_at(i)->Render(screensize, buttonrect);
	}
}