#ifndef __HEROSTATE_H__
#define __HEROSTATE_H__

#include "Hero.h"

/////////////////////////////////////////////////////////////
// Idle
/////////////////////////////////////////////////////////////
class Hero::IdleState : public Hero::StateBase {
public:
	IdleState(Hero& context) : StateBase(context) {};
	virtual void Initialize();
	virtual void Jump(const CIwFVec2& impulse);
};

/////////////////////////////////////////////////////////////
// Jumping
/////////////////////////////////////////////////////////////
class Hero::JumpingState : public Hero::StateBase {
public:
	JumpingState(Hero& context) : StateBase(context) {};
	virtual void Initialize();
	virtual void Attack();
	virtual void Collide(Body& body);
	virtual void Update(uint16 timestep);
};

/////////////////////////////////////////////////////////////
// Smashing
/////////////////////////////////////////////////////////////
class Hero::SmashingState : public Hero::StateBase {
public:
	SmashingState(Hero& context) : StateBase(context) {};
	virtual void Initialize();
	virtual void Collide(Body& body);
	virtual void Update(uint16 timestep);
};

#endif
