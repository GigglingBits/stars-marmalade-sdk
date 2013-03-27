#ifndef __VERTEXSTREAMWORLD_H__
#define __VERTEXSTREAMWORLD_H__

#include "IwGeom.h"
#include "VertexStream.h"

class VertexStreamWorld : public VertexStream<CIwFVec2> {
public:
	void SetRect(float x, float y, float w, float h);
};

#endif
