#ifndef __STARMOTIONSTATE_H__
#define __STARMOTIONSTATE_H__

#include "Star.h"

/////////////////////////////////////////////////////////////
// Initial
/////////////////////////////////////////////////////////////
class Star::InitialState : public Star::StateBase {
public:
	InitialState(Star& context) : StateBase(context) {};
	virtual void Initialize();
	virtual void Update(uint16 timestep);
};

/////////////////////////////////////////////////////////////
// Passive
/////////////////////////////////////////////////////////////
class Star::PassiveState : public Star::StateBase {
public:
	PassiveState(Star& context) : StateBase(context) {};
	virtual void Initialize();
	virtual void FollowPath();
	virtual void Update(uint16 timestep);
};

/////////////////////////////////////////////////////////////
// Retracting
/////////////////////////////////////////////////////////////
class Star::RetractingState : public Star::StateBase {
public:
	RetractingState(Star& context) : StateBase(context) {};
	virtual void Initialize();
	virtual void Passify();
	virtual void FollowPath();
	virtual void Collide(Body& body);
	virtual void Update(uint16 timestep);
};

/////////////////////////////////////////////////////////////
// Following path
/////////////////////////////////////////////////////////////
class Star::FollowState : public Star::StateBase {
public:
	FollowState(Star& context) : StateBase(context) {};
	virtual void Initialize();
	virtual void Passify();
	virtual void FollowPath();
	virtual void Collide(Body& body);
	virtual void Update(uint16 timestep);
};

/////////////////////////////////////////////////////////////
// Falling
/////////////////////////////////////////////////////////////
class Star::FallingState : public Star::StateBase {
public:
	FallingState(Star& context) : StateBase(context) {};
	virtual void Initialize();
};

#endif
