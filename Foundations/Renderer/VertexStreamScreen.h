#ifndef __VERTEXSTREAMSCREEN_H__
#define __VERTEXSTREAMSCREEN_H__

#include "VertexStream.h"
#include "IwGxTypes.h"

class VertexStreamScreen : public VertexStream<CIwSVec2> {
public:
	void SetRect(const CIwRect& rect);
	void SetRect(int16 x, int16 y, int16 w, int16 h);
};

#endif
