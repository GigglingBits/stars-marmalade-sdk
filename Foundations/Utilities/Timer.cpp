#include "Timer.h"
#include "time.h"

Timer::Timer() {
	Stop();
}

void Timer::Start(uint32 milliseconds, long data) {
	m_bStarted = true;
	m_iTimeRemaining = milliseconds;
	m_lUserData = data;
}

void Timer::Start(uint32 minmilliseconds, uint32 maxmilliseconds, long data) {
	// idea from: http://www.cplusplus.com/forum/general/14192/
	srand((unsigned)time(0));
	int range = (maxmilliseconds - minmilliseconds);
	Start(minmilliseconds + int((range * rand()) / (RAND_MAX + 1.0)), data);	
}

void Timer::Stop() {
	m_iTimeRemaining = 0;
	m_lUserData = 0;
	m_bStarted = false;		
}

void Timer::Update(uint32 timestep) {
	if (!m_bStarted) {
		return;
	}

	if (m_iTimeRemaining > timestep) {
		m_iTimeRemaining -= timestep;
	} else {
		long data = m_lUserData;
		Stop();
		ElapsedEvent.Invoke(*this, data);
	}
}
