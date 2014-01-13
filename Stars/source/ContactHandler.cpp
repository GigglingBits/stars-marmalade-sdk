#include "ContactHandler.h"
#include "Body.h"

ContactHandler::ContactHandler(World& world) :
m_rxWorld(world) {
	m_rxWorld.SetContactListener(*this);
}

ContactHandler::~ContactHandler() {
	m_rxWorld.RemoveContactListener();
}

void ContactHandler::OnContact(Body* bodya, Body* bodyb, bool issensorcollision, float approachvelocity, const CIwFVec2& collisionpoint) {
	Collide(*bodya, *bodyb, issensorcollision, collisionpoint, approachvelocity);
	Collide(*bodyb, *bodya, issensorcollision, collisionpoint, approachvelocity);
}

void ContactHandler::Collide(Body& body1, Body& body2, bool issensorcollision, const CIwFVec2 collisionpoint, float approachvelocity) {
	IW_CALLSTACK_SELF;
	IwTrace(MYAPP, ("Colliding: %s(%s) -> %s(%s)", body1.GetTypeName(), body1.GetId().c_str(), body2.GetTypeName(), body2.GetId().c_str()));
	
	// do body-internal actions (state transitions, texture changes, etc).
	body1.Collide(body2);
}
