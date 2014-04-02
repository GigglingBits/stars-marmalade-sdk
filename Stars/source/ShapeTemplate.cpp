#include "ShapeTemplate.h"
#include "World.h"
#include "IwDebug.h"
#include "Debug.h"

ShapeTemplate::ShapeTemplate() {
	m_eShapeType = eUnknown;
	m_dDiameter = 0.0f;
	m_dWidth = 0.0f;
	m_dHeight = 0.0f;
	m_iVertexCount = 0;
	memset(m_axVertices, 0, sizeof(m_axVertices));
}

ShapeTemplate::~ShapeTemplate() {
	Reset();
}

b2Shape* ShapeTemplate::CreatePhysicsShapeDef() const {
	b2Shape* shape = NULL;

	switch (m_eShapeType) {
	case eCircle:
		{
			b2CircleShape* circle = new b2CircleShape();
			circle->m_radius = (float32)(m_dDiameter/2.0f);
			circle->m_p = b2Vec2(0.0f, 0.0f);
			shape = circle;
		}
		break;
	case eRectangle:
		{
			float32 hx = (float32)(m_dWidth/2.0f);
			float32 hy = (float32)(m_dHeight/2.0f);
			b2PolygonShape* polygon = new b2PolygonShape();
			polygon->SetAsBox(hx, hy, b2Vec2(-hx, -hy), 0.0f);
			shape = polygon;
		}
		break;
	case ePolygon:
		{
			b2PolygonShape* polygon = new b2PolygonShape();
			polygon->Set(m_axVertices, m_iVertexCount);
			shape = polygon;
		}
		break;
	case eChain:
		{
			b2ChainShape* chain = new b2ChainShape();
			chain->CreateChain(m_axVertices, m_iVertexCount);
			shape = chain;
		}
		break;
	default:
		IwAssertMsg(MYAPP, false,("Unknown shape type. Cannot create instance."));
		break;
	}

	return shape;
}

void ShapeTemplate::CreateAABB(CIwFVec2 verts[4]) const {
	IW_CALLSTACK_SELF;
	
	// use box2d to evaluate the shape data
	if (b2Shape* shape = CreatePhysicsShapeDef()) {
		b2AABB aabb;
		b2Transform txidentity;
		txidentity.SetIdentity();
		shape->ComputeAABB(&aabb, txidentity, 0);
		delete shape;

		verts[0].x = aabb.lowerBound.x;
		verts[0].y = aabb.lowerBound.y;
		verts[1].x = aabb.upperBound.x;
		verts[1].y = aabb.lowerBound.y;
		verts[2].x = aabb.upperBound.x;
		verts[2].y = aabb.upperBound.y;
		verts[3].x = aabb.lowerBound.x;
		verts[3].y = aabb.upperBound.y;
	} else {
		IwAssertMsg(MYAPP, false, ("Unable to calculate shape AABB; Setting to 0:0 size."));
		verts[0] = CIwSVec2::g_Zero;
		verts[1] = CIwSVec2::g_Zero;
		verts[2] = CIwSVec2::g_Zero;
		verts[3] = CIwSVec2::g_Zero;
	}
}

void ShapeTemplate::Reset() {
	m_eShapeType = eUnknown;
}

void ShapeTemplate::SetRectangle(double width, double height) {
	Reset();
	m_eShapeType = eRectangle;
	m_dWidth = width;
	m_dHeight = height;
}

void ShapeTemplate::SetCircle(double diameter) {
	Reset();
	m_eShapeType = eCircle;
	m_dDiameter = diameter;
}

void ShapeTemplate::SetPolygon(int count, float* xpoints, float* ypoints) {
	IwAssertMsg(MYAPP, count <= SHAPE_MAX_VERTS, ("Shape has too many vertices: %i", count));
	Reset();
	m_eShapeType = ePolygon;
	SetVertices(count, xpoints, ypoints);
}

void ShapeTemplate::SetChain(int count, float* xpoints, float* ypoints) {
	IwAssertMsg(MYAPP, count <= SHAPE_MAX_VERTS, ("Shape has too many vertices: %i", count));
	Reset();
	m_eShapeType = eChain;
	SetVertices(count, xpoints, ypoints);
}

void ShapeTemplate::SetVertices(int count, float* xpoints, float* ypoints) {
	IwAssertMsg(MYAPP, count <= SHAPE_MAX_VERTS, ("Shape has too many vertices: %i", count));
	m_iVertexCount = count;
	for (int i = 0; i < count; i++) {
		m_axVertices[i].Set(xpoints[i], ypoints[i]);
	}
}
