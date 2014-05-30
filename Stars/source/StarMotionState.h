#ifndef __STARMOTIONSTATE_H__
#define __STARMOTIONSTATE_H__

#include "Star.h"

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
// Follow path
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
// Recover
/////////////////////////////////////////////////////////////
class Star::RecoverState : public Star::MotionStateBase {
private:
	uint32 m_uiRemainingTime;
	
public:
	RecoverState(Star& context) : MotionStateBase(context), m_uiRemainingTime(0) {};
	virtual void Initialize();
	virtual void FollowPath();
	virtual void Update(uint16 timestep);
};

#endif
