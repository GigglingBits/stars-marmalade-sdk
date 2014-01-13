#ifndef __CONTACTHANDLER_H__
#define __CONTACTHANDLER_H__

#include "World.h"
#include "ContactListener.h"

class ContactHandler : public ContactListener {
private:
	World& m_rxWorld;
	
public:
	ContactHandler(World& world);
	~ContactHandler();
	
private:
	virtual void OnContact(Body* bodya, Body* bodyb, bool issensorcollision, float approachvelocity, const CIwFVec2& collisionpoint);

	void Collide(Body& body1, Body& body2, bool issensorcollision, const CIwFVec2 collisionpoint, float approachvelocity);
};

#endif
