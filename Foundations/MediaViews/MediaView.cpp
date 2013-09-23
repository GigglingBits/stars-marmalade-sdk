#include "MediaView.h"

MediaView::MediaView() : m_xSize(CIwVec2::g_Zero), m_xPosition(CIwVec2::g_Zero) {
}

void MediaView::SetPosition(const CIwVec2& pos, const CIwVec2& size) {
	m_xPosition = pos;
	m_xSize = size;

	InvalidateLayout();
}

const CIwVec2& MediaView::GetPosition() {
	return m_xPosition;
}

const CIwVec2& MediaView::GetSize() {
	return m_xSize;
}
