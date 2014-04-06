#ifndef __CONTACTLISTENER_H__
#define __CONTACTLISTENER_H__

#include "Box2D/Box2D.h"
#include "MulticastEvent.h"
#include "IwGeom.h"

class Body;

class ContactListener : public b2ContactListener {
public:
	virtual void BeginContact(b2Contact* contact);
	//virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	//virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

private:
	inline Body* GetActorA(const b2Contact* contact);
	inline Body* GetActorB(const b2Contact* contact);
	inline Body* GetActor(const b2Fixture* fixture);
	inline Body* GetActor(const b2Body* body);

	b2Vec2 CalculateAverage(const b2Vec2& vectora, const b2Vec2& vectorb);
	float CalculateApproachVelocity(b2Body* bodya, b2Body* bodyb, const b2Vec2& refpoint, const b2Vec2& normal);

protected:
	virtual void OnContact(Body* bodya, Body* bodyb, bool issensorcollision, float approachvelocity, const CIwFVec2& collisionpoint);
	
public:
	struct CollisionEventArgs {
		Body* bodya;
		Body* bodyb;
		bool issensorcollision;
		float approachvelocity;
		CIwFVec2 collisionpoint;
	};
	MulticastEvent<ContactListener, CollisionEventArgs> CollisionEvent;
};

#endif
