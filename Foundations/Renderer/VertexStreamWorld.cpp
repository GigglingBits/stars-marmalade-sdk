#include "VertexStreamWorld.h"

void VertexStreamWorld::SetRect(float x, float y, float w, float h) {
	VertexStream<CIwFVec2>::SetRect<float>(x, y, w, h);
}
