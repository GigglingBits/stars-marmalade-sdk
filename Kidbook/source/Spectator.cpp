#include "Spectator.h"

Spectator::Spectator(PageMap& map) : m_rxMap(map) {
	srand(time(NULL));
	m_iColCursor = rand() % map.GetColCount();
	m_iRowCursor = rand() % map.GetRowCount();

	memset(m_apxPages, 0, sizeof(m_apxPages));	
	UpdatePageReferences();

	m_eLastMoveDir = eCenter;

	ActivateAll();
}

inline void Spectator::UpdatePageReferences() {
	m_apxPages[eCenter] =		&m_rxMap.GetImage(m_iColCursor, m_iRowCursor);
	m_apxPages[eRight] =		&m_rxMap.GetImage(m_iColCursor + 1, m_iRowCursor); 
	m_apxPages[eLeft] =			&m_rxMap.GetImage(m_iColCursor - 1, m_iRowCursor); 
	m_apxPages[eUp] =			&m_rxMap.GetImage(m_iColCursor, m_iRowCursor + 1); 
	m_apxPages[eUpRight] =		&m_rxMap.GetImage(m_iColCursor + 1, m_iRowCursor + 1); 
	m_apxPages[eUpLeft] =		&m_rxMap.GetImage(m_iColCursor - 1, m_iRowCursor + 1); 
	m_apxPages[eDown] =			&m_rxMap.GetImage(m_iColCursor, m_iRowCursor - 1); 
	m_apxPages[eDownRight] =	&m_rxMap.GetImage(m_iColCursor + 1, m_iRowCursor - 1); 
	m_apxPages[eDownLeft] =		&m_rxMap.GetImage(m_iColCursor - 1, m_iRowCursor - 1); 
}

void Spectator::ActivateAll() {
	SetActive(eUpLeft, true);
	SetActive(eUp, true);
	SetActive(eUpRight, true);
	SetActive(eLeft, true);
	SetActive(eCenter, true);
	SetActive(eRight, true);
	SetActive(eDownLeft, true);
	SetActive(eDown, true);
	SetActive(eDownRight, true);
}

inline bool Spectator::SetActive(Direction dir, bool active) {
	if (m_apxPages[dir]) {
		if (active && m_apxPages[dir]->IsActive()) {
			return false;
		}
		if (!active && !m_apxPages[dir]->IsActive()) {
			return false;
		}
		m_apxPages[dir]->SetActive(active);
		return true;
	}
	return false;
}

void Spectator::Move(Direction dir) {
	if (DIR_IS_UP(dir)) {
		SetActive(eDownRight, false);
		SetActive(eDown, false);
		SetActive(eDownLeft, false);
		m_iRowCursor++;
	} else if (DIR_IS_DOWN(dir)) {
		SetActive(eUpLeft, false);
		SetActive(eUp, false);
		SetActive(eUpRight, false);
		m_iRowCursor--;
	}
	if (DIR_IS_RIGHT(dir)) {
		SetActive(eUpLeft, false);
		SetActive(eLeft, false);
		SetActive(eDownLeft, false);
		m_iColCursor++;
	} else if (DIR_IS_LEFT(dir)) {
		SetActive(eUpRight, false);
		SetActive(eRight, false);
		SetActive(eDownRight, false);
		m_iColCursor--;
	}

	UpdatePageReferences();
	ActivateAll();

	m_eLastMoveDir = dir;
}

Page* Spectator::GetPage(Direction dir) {
	return m_apxPages[dir];
}

bool Spectator::OnUpdate(bool first) {
	// currently visible
	if (UpdateSingle(eCenter, first)) return true;

	// predict next move
	Direction next = m_eLastMoveDir == eCenter ? eRight : m_eLastMoveDir;
	if (UpdateSingle(next, first)) return true;

	// rest
	if (eUpLeft != next && UpdateSingle(eUpLeft, first)) return true;
	if (eUp != next && UpdateSingle(eUp, first)) return true;
	if (eUpRight != next && UpdateSingle(eUpRight, first)) return true;
	if (eLeft != next && UpdateSingle(eLeft, first)) return true;
	if (eRight != next && UpdateSingle(eRight, first)) return true;
	if (eDownLeft != next && UpdateSingle(eDownLeft, first)) return true;
	if (eDown != next && UpdateSingle(eDown, first)) return true;
	if (eDownRight!= next && UpdateSingle(eDownRight, first)) return true;
	return false;
}

inline bool Spectator::UpdateSingle(Direction dir, bool first) {
	if (m_apxPages[dir]) {
		return m_apxPages[dir]->Update(first);
	}
	return false;
}

void Spectator::OnRender(const CIwSVec2& screensize, const CIwRect& itemregion) {
	RenderSingle(eUpLeft, screensize, itemregion);
	RenderSingle(eUp, screensize, itemregion);
	RenderSingle(eUpRight, screensize, itemregion);
	RenderSingle(eLeft, screensize, itemregion);
	RenderSingle(eCenter, screensize, itemregion);
	RenderSingle(eRight, screensize, itemregion);
	RenderSingle(eDownLeft, screensize, itemregion);
	RenderSingle(eDown, screensize, itemregion);
	RenderSingle(eDownRight, screensize, itemregion);
}
		
inline void Spectator::RenderSingle(Direction dir, const CIwSVec2& screensize, const CIwRect& itemregion) {
	// there are 3x3 images to be placed into itemregion
	CIwRect rect(itemregion);
	rect.w /= 3;
	rect.h /= 3;

	if (DIR_IS_DOWN(dir)) {
		rect.y += rect.h * 2;
	} else if (!DIR_IS_UP(dir)) {
		rect.y += rect.h;
	}
	if (DIR_IS_RIGHT(dir)) {
		rect.x += rect.w * 2;
	} else if (!DIR_IS_LEFT(dir)) {
		rect.x += rect.w;
	}
	if (m_apxPages[dir]) {
		m_apxPages[dir]->Render(screensize, rect);
	}
}
