#include "RayCaster.h"

RayCaster::RayCaster(World& world) : m_rxWorld(world) {
}

b2Vec2 RayCaster::Convert(CIwFVec2 vec) {
	return b2Vec2(vec.x, vec.y);
}

bool RayCaster::RayHitTest(CIwFVec2 start, CIwFVec2 end) {
	m_bHit = false;
	m_rxWorld.GetWorld().RayCast(this, Convert(start), Convert(end));
	return m_bHit;
}

float32 RayCaster::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
	// filter irrelevant bodies
	if (fixture->IsSensor()) {
		return fraction;
	};

	// got an actual hit
	m_bHit = true;
	return 0.0f;
}
