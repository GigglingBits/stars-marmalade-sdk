#include "World.h"

int World::s_iReferenceCounter = 0;

b2World* World::s_pxWorld = NULL;
b2Body* World::s_pxAnchorBody = NULL;

b2Vec2 World::s_xGravity(0.0f, -9.81f);

World::World() {
	if (s_iReferenceCounter == 0) {
		Create();
	}
	s_iReferenceCounter++;
}

World::~World() {
	s_iReferenceCounter--;
	if (s_iReferenceCounter == 0) {
		Destroy();
	}
}

void World::SetDefaultGravity(float x, float y) {
	IwAssertMsg(MYAPP, !s_pxWorld, ("The default gravity should not be changed while world instances already exist as it may lead to unintended results."));

	s_xGravity.x = x;
	s_xGravity.y = y;
}

void World::Create() {
	// world
    s_pxWorld = new b2World(s_xGravity);	 

	// add anchor (can be used as refernce body)
	b2BodyDef def;
	def.position.SetZero();
	def.fixedRotation = true;
	def.type = b2_staticBody;
	s_pxAnchorBody = s_pxWorld->CreateBody(&def);
}

void World::Destroy() {
	// clear anchor
	s_pxWorld->DestroyBody(s_pxAnchorBody);
	s_pxAnchorBody = NULL;

	// world
    delete s_pxWorld;	 
    s_pxWorld = NULL;
}

b2World& World::GetWorld() {
	return *s_pxWorld;
}

b2Body& World::GetAnchorBody() {
	return *s_pxAnchorBody;
}

void World::SetContactListener(ContactListener* listener) {
	GetWorld().SetContactListener(listener);
}
