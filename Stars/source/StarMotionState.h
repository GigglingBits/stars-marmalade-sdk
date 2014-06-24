#ifndef __STARMOTIONSTATE_H__
#define __STARMOTIONSTATE_H__

#include "Star.h"

/////////////////////////////////////////////////////////////
// Rising
/////////////////////////////////////////////////////////////
class Star::RisingState : public Star::MotionStateBase {
public:
	RisingState(Star& context) : MotionStateBase(context) {};
	virtual void Initialize();
	virtual void FollowPath();
	virtual void Update(uint16 timestep);
};

/////////////////////////////////////////////////////////////
// Retracting
/////////////////////////////////////////////////////////////
class Star::RetractingState : public Star::MotionStateBase {
public:
	RetractingState(Star& context) : MotionStateBase(context) {};
	virtual void Initialize();
	virtual void FollowPath();
	virtual void Collide(Body& body);
	virtual void Update(uint16 timestep);
};

/////////////////////////////////////////////////////////////
// Following path
/////////////////////////////////////////////////////////////
class Star::FollowState : public Star::MotionStateBase {
public:
	FollowState(Star& context) : MotionStateBase(context) {};
	virtual void Initialize();
	virtual void FollowPath();
	virtual void Collide(Body& body);
	virtual void Update(uint16 timestep);
};

/////////////////////////////////////////////////////////////
// Falling
/////////////////////////////////////////////////////////////
class Star::FallingState : public Star::MotionStateBase {
public:
	FallingState(Star& context) : MotionStateBase(context) {};
	virtual void Initialize();
};

#endif
