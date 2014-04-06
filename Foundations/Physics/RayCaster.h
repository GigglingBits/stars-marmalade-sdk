#ifndef __RAYCASTER_H__
#define __RAYCASTER_H__

#include "Box2D/Box2D.h"
#include "IwGeom.h"
#include "World.h"

class RayCaster : private b2RayCastCallback {
private:
	World& m_rxWorld;
	bool m_bHit;

public:
	RayCaster(World& world);

	bool RayHitTest(CIwFVec2 start, CIwFVec2 end);

private:
	b2Vec2 Convert(CIwFVec2 vec);
	virtual float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
};

#endif
