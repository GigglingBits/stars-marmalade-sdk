#include "VertexStreamScreen.h"

void VertexStreamScreen::SetRect(const CIwRect& rect) {
	SetRect(rect.x, rect.y, rect.w, rect.h);
}

void VertexStreamScreen::SetRect(int16 x, int16 y, int16 w, int16 h) {
	VertexStream<CIwSVec2>::SetRect<int16>(x, y, w, h);
}
