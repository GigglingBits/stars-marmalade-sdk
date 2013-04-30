#ifndef __SHAPETEMPLATE_H__
#define __SHAPETEMPLATE_H__

#include "Box2D.h"
#include "IwGeomFVec2.h"

#define SHAPE_MAX_VERTS 16

class ShapeTemplate {
private:
	enum eShapeType {
		eUnknown,
		eCircle,
		eRectangle,
		eChain,
		ePolygon
	};

	eShapeType m_eShapeType;
	double m_dDiameter;
	double m_dWidth;
	double m_dHeight;
	int m_iVertexCount;
	b2Vec2 m_axVertices[SHAPE_MAX_VERTS];

public:
	ShapeTemplate();
	~ShapeTemplate();

	void SetRectangle(double width, double height);
	void SetCircle(double diameter);
	void SetPolygon(int count, float* xpoints, float* ypoints);
	void SetChain(int count, float* xpoints, float* ypoints);

	b2Shape* CreatePhysicsShapeDef() const;
	void CreateAABB(CIwFVec2 verts[4]) const;

private:
	void SetVertices(int count, float* xpoints, float* ypoints);
	void Reset();
};

#endif
