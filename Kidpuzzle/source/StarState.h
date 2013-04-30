#ifndef __STARSTATE_H__
#define __STARSTATE_H__

#include "Star.h"

/////////////////////////////////////////////////////////////
// Idle
/////////////////////////////////////////////////////////////
class Star::IdleState : public Star::StateBase {
public:
	IdleState(Star& context) : StateBase(context) {};
	virtual void Initialize();
	virtual void Collide(Body& body);
};

/////////////////////////////////////////////////////////////
// Jumping
/////////////////////////////////////////////////////////////
class Star::JumpingState : public Star::StateBase {
public:
	JumpingState(Star& context) : StateBase(context) {};
	virtual void Initialize();
	virtual void Attack();
	virtual void Collide(Body& body);
	virtual void Update(uint16 timestep);
};

/////////////////////////////////////////////////////////////
// Smashing
/////////////////////////////////////////////////////////////
class Star::SmashingState : public Star::StateBase {
public:
	SmashingState(Star& context) : StateBase(context) {};
	virtual void Initialize();
	virtual void Collide(Body& body);
	virtual void Update(uint16 timestep);
};

#endif
