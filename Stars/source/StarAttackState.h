#ifndef __STARATTACKSTATE_H__
#define __STARATTACKSTATE_H__

#include "Star.h"

/////////////////////////////////////////////////////////////
// Peaceful
/////////////////////////////////////////////////////////////
class Star::PeacefulState : public Star::AttackStateBase {
public:
	PeacefulState(Star& context) : AttackStateBase(context) {};
	virtual void Initialize();
	
	virtual void BeginBlock();
	virtual void BeginAttack();

	virtual void Collide(Body& body);
	virtual void Update(uint16 timestep);
};

/////////////////////////////////////////////////////////////
// Block
/////////////////////////////////////////////////////////////
class Star::BlockState : public Star::AttackStateBase {
public:
	BlockState(Star& context) : AttackStateBase(context) {};
	virtual void Initialize();
	
	virtual void EndBlock();
	
	virtual void Update(uint16 timestep);
};

/////////////////////////////////////////////////////////////
// Attack
/////////////////////////////////////////////////////////////
class Star::AttackState : public Star::AttackStateBase {
public:
	AttackState(Star& context) : AttackStateBase(context) {};
	virtual void Initialize();
	
	virtual void EndAttack();
	
	virtual void Update(uint16 timestep);
};

#endif
