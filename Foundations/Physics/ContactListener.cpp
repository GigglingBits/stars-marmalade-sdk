#include "ContactListener.h"

void ContactListener::BeginContact(b2Contact* contact) {
	// only handle sensor contacts; other contacts are dealt with in PreSolve
	if (!contact->GetFixtureA()->IsSensor() && !contact->GetFixtureB()->IsSensor()) {
		return;
	}

	// sensor contacts do not have manifolds; hence this special treatment
	CollisionEventArgs args;
	args.issensorcollision = true;
	IwAssertMsg(MYAPP, 
		contact->GetManifold()->pointCount == 0, 
		("Sensor contacts are assumed not to have a manifold! Assumption failed. \
		 The contact is possibly not a sensor contact, but will be treated as such!"));

	// identify bodies
	b2Body* b2bodya = contact->GetFixtureA()->GetBody();
	b2Body* b2bodyb = contact->GetFixtureB()->GetBody();

	b2Vec2 b2bodyapos = b2bodya->GetWorldCenter();
	b2Vec2 b2bodybpos = b2bodyb->GetWorldCenter();

	// find collision position
	b2Vec2 b2collisionpoint = CalculateAverage(b2bodyapos, b2bodybpos);

	// calculate relative velocity
	float approachvelocity = CalculateApproachVelocity(
		b2bodya, b2bodyb,
		b2collisionpoint,
		b2bodybpos - b2bodyapos);

	// call the collision subscribers
	Body* bodya = GetActor(b2bodya);
	Body* bodyb = GetActor(b2bodyb);
	CIwFVec2 collisionpoint(b2collisionpoint.x, b2collisionpoint.y);
	OnContact(bodya, bodyb, false, approachvelocity, collisionpoint);
}

//void ContactListener::EndContact(b2Contact* contact) {
//}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
	// only consider new contacts;
	// return on old contacts
	b2PointState state1[2], state2[2];
	b2GetPointStates(state1, state2, oldManifold, contact->GetManifold());
	if (state2[0] != b2_addState) {
		return;
	}

	// get bodies
	b2Body* b2bodya = contact->GetFixtureA()->GetBody();
	b2Body* b2bodyb = contact->GetFixtureB()->GetBody();

	// assert for sensors
	const b2Manifold* manifold = contact->GetManifold();
	int pointcount = manifold->pointCount;
	if (pointcount == 0) {
		IwAssertMsg(MYAPP, false, ("Contact has no manifold. This should only happen for sensors. And sensors should never trigger this function."));
		return;
	}

	// find collision position
	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);
	b2Vec2 b2collisionpoint;
	if (pointcount == 1) {
		b2collisionpoint = worldManifold.points[0];
	} else {
		b2collisionpoint = CalculateAverage(worldManifold.points[0], worldManifold.points[1]);
	}
	
	// calculate relative velocity
	float approachvelocity = CalculateApproachVelocity(
		b2bodya, b2bodyb,
		worldManifold.points[0], 
		worldManifold.normal);

	// call the collision subscribers
	Body* bodya = GetActor(b2bodya);
	Body* bodyb = GetActor(b2bodyb);
	CIwFVec2 collisionpoint(b2collisionpoint.x, b2collisionpoint.y);
	OnContact(bodya, bodyb, false, approachvelocity, collisionpoint);
}

//void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
//}

Body* ContactListener::GetActorA(const b2Contact* contact) {
	return GetActor(contact->GetFixtureA());
}

Body* ContactListener::GetActorB(const b2Contact* contact) {
	return GetActor(contact->GetFixtureB());
}

Body* ContactListener::GetActor(const b2Fixture* fixture) {
	return (Body*)(fixture->GetUserData());
}

Body* ContactListener::GetActor(const b2Body* body) {
	return (Body*)(body->GetUserData());
}

b2Vec2 ContactListener::CalculateAverage(const b2Vec2& vectora, const b2Vec2& vectorb) {
	b2Vec2 vector(vectora - vectorb);
	vector *= 0.5;
	return vector + vectorb;
}

float ContactListener::CalculateApproachVelocity(b2Body* bodya, b2Body* bodyb, const b2Vec2& refpoint, const b2Vec2& normal) {
	b2Vec2 vA = bodya->GetLinearVelocityFromWorldPoint(refpoint);
	b2Vec2 vB = bodyb->GetLinearVelocityFromWorldPoint(refpoint);
	return b2Dot(vB - vA, normal);	
}

void ContactListener::OnContact(Body* bodya, Body* bodyb, bool issensorcollision, float approachvelocity, const CIwFVec2& collisionpoint) {
	CollisionEventArgs args;
	args.bodya = bodya;
	args.bodyb = bodyb;
	args.issensorcollision = false;
	args.approachvelocity = approachvelocity;
	args.collisionpoint = collisionpoint;
	CollisionEvent.Invoke(*this, args);
}
