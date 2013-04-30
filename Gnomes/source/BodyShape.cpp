#include "BodyShape.h"

CIwFVec2* BodyShape::GetShapeVerts() {
	return m_xShapeVerts.GetVerts();
}

int BodyShape::GetShapeVertCount() {
	return m_xShapeVerts.GetVertCount();
}

CIwFVec2* BodyShape::GetBoundigBoxVerts() {
	return m_xBoundingBoxVerts.GetVerts();
}

int BodyShape::GetBoundigBoxVertCount() {
	return m_xBoundingBoxVerts.GetVertCount();
}

void BodyShape::SetVertices(const b2Transform& transform, const b2Shape& shape) {
	SetBoundigBoxVerts(transform, shape);
	SetShapeVerts(transform, shape);
}

void BodyShape::SetShapeVerts(const b2Transform& transform, const b2Shape& shape) {
	if (shape.GetType() == b2Shape::e_polygon) {
		b2PolygonShape& polygon = (b2PolygonShape&)shape;
		SetShapeVerts(transform, polygon);

	} else if (shape.GetType() == b2Shape::e_chain) {
		b2ChainShape& chain = (b2ChainShape&)shape;
		SetShapeVerts(transform, chain);

	} else if (shape.GetType() == b2Shape::e_circle) {
		b2CircleShape& circle = (b2CircleShape&)shape;
		SetShapeVerts(transform, circle);

	} else {
		// no specific shape; we just copy the AABB
		IwAssertMsg(MYAPP, false, ("Invalid shape type detected. The AABB is used instead."));
		m_xShapeVerts.SetVerts(m_xBoundingBoxVerts.GetVerts(), m_xBoundingBoxVerts.GetVertCount(), false);
	}
}

void BodyShape::SetShapeVerts(const b2Transform& transform, const b2PolygonShape& polygon) {
	// allocate verts
	int count = polygon.GetVertexCount();
	CIwFVec2* verts = new CIwFVec2[count + 1];

	// copy/transform vertices
	for (int i = 0; i < count; i++) {
		b2Vec2 vertex = b2Mul(transform, polygon.GetVertex(/*m_iShapeVertCount - 1 - */i));
		verts[i].x = vertex.x;
		verts[i].y = vertex.y;
	}

	// close the shape
	verts[count] = verts[0];

	// store vertices
	m_xShapeVerts.SetVerts(verts, count + 1 /* closed shape */, true);
}

void BodyShape::SetShapeVerts(const b2Transform& transform, const b2CircleShape& circle) {
	//  Note: Circle calculation... is in many cases too precise... 
	//		  in most cases it is anyway a rectangular picture, so AABB would be enough

	IwAssertMsg(MYAPP, circle.GetVertexCount() == 1, ("Assumption wrong: The circles are not represented as polygons. The single vertex represents the center."));

	// allocate verts
	const int count = 10;
	CIwFVec2* verts = new CIwFVec2[count + 1];

	// generate vertices
	b2Vec2 center = circle.GetVertex(0);
	for (int i = 0; i < count; i++) {
		float angle = 2.0f * b2_pi * (float)i / (float)count;
		float32 c = cosf(angle), s = sinf(angle);
		b2Vec2 vertex = b2Mul(transform, center + b2Vec2(c * circle.m_radius, s * circle.m_radius));

		verts[i].x = vertex.x;
		verts[i].y = vertex.y;
	}

	// close the shape
	verts[count] = verts[0];

	// store vertices
	m_xShapeVerts.SetVerts(verts, count + 1 /* closed shape */, true);
}

void BodyShape::SetShapeVerts(const b2Transform& transform, const b2ChainShape& chain) {
	// allocate verts
	int count = chain.GetChildCount();
	CIwFVec2* verts = new CIwFVec2[count + 1];

	// copy/transform vertices
	b2EdgeShape edge;
	for (int i = 0; i < count; i++) {
		chain.GetChildEdge(&edge, /*m_iShapeVertCount - 1 - */i);			
		IwAssertMsg(MYAPP, (!edge.m_hasVertex0 && i == 0) || (edge.m_hasVertex0 && i > 0), ("First edge must not have a preceeding edge; all others do."));
		IwAssertMsg(MYAPP, (edge.m_hasVertex3 && i < count - 1) || (!edge.m_hasVertex3 && i == count - 1), ("Last edge must not have a subsequent edge; all others do."));

		b2Vec2 vertex = b2Mul(transform, edge.m_vertex1);
		verts[i].x = vertex.x;
		verts[i].y = vertex.y;

		if (i == count -1) {
			// last iteration
			vertex = b2Mul(transform, edge.m_vertex2);
			verts[i + 1].x = vertex.x;
			verts[i + 1].y = vertex.y;
		}
	}

	// store vertices
	m_xShapeVerts.SetVerts(verts, count + 1  /* end point */, true);
}

void BodyShape::SetBoundigBoxVerts(const b2Transform& transform, const b2Shape& shape) {
	const int vertcount = 4;
	CIwFVec2* verts = new CIwFVec2[vertcount];

	b2AABB aabb;
	b2Transform txidentity;
	txidentity.SetIdentity();
	shape.ComputeAABB(&aabb, txidentity, 0);

	b2Vec2 vertex = b2Mul(transform, aabb.upperBound);
	verts[2].x = vertex.x;
	verts[2].y = vertex.y;

	vertex = b2Mul(transform, b2Vec2(aabb.lowerBound.x, aabb.upperBound.y));
	verts[3].x = vertex.x;
	verts[3].y = vertex.y;

	vertex = b2Mul(transform, aabb.lowerBound);
	verts[0].x = vertex.x;
	verts[0].y = vertex.y;

	vertex = b2Mul(transform, b2Vec2(aabb.upperBound.x, aabb.lowerBound.y));
	verts[1].x = vertex.x;
	verts[1].y = vertex.y;

	m_xBoundingBoxVerts.SetVerts(verts, vertcount, true);
}
