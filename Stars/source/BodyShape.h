#ifndef __BODYSHAPE_H__
#define __BODYSHAPE_H__

#include "Box2D/Box2D.h"
#include "VertexStreamWorld.h"

class BodyShape {
private:
	VertexStreamWorld m_xShapeVerts;
	VertexStreamWorld m_xBoundingBoxVerts;

public:
	CIwFVec2* GetShapeVerts();
	int GetShapeVertCount();

	CIwFVec2* GetBoundigBoxVerts();
	int GetBoundigBoxVertCount();

	void SetVertices(const b2Transform& transform, const b2Shape& shape);

private:
	void SetShapeVerts(const b2Transform& transform, const b2PolygonShape& polygon);
	void SetShapeVerts(const b2Transform& transform, const b2CircleShape& circle);
	void SetShapeVerts(const b2Transform& transform, const b2ChainShape& chain);	
	void SetShapeVerts(const b2Transform& transform, const b2Shape& shape);

	void SetBoundigBoxVerts(const b2Transform& transform, const b2Shape& shape);
};

#endif
	