#ifndef __STARMOTIONSTATE_H__
#define __STARMOTIONSTATE_H__

#include "Star.h"

/////////////////////////////////////////////////////////////
// Idle
/////////////////////////////////////////////////////////////
class Star::RetractingState : public Star::MotionStateBase {
public:
	RetractingState(Star& context) : MotionStateBase(context) {};
	virtual void Initialize();
	virtual void FollowPath();
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
	virtual void Update(uint16 timestep);
};

#endif
