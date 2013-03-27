#ifndef _TIMER_H_ 
#define _TIMER_H_ 

#include "IwGeom.h"
#include "Event.h"

class Timer {
private:
	bool m_bStarted;
	uint32 m_iTimeRemaining;
	long m_lUserData;

public:
	Timer();

	void Start(uint32 milliseconds, long data);
	void Start(uint32 minmilliseconds, uint32 maxmilliseconds, long data);
	void Stop();
	void Update(uint32 timestep);

	Event<Timer, long> ElapsedEvent;
};

#endif
