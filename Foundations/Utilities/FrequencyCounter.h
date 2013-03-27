#ifndef __FREQUENCYCOUNTER_H__ 
#define __FREQUENCYCOUNTER_H__ 

#include "IwTypes.h"

#define FRQCNT_STORE_SIZE 32
#define FRQCNT_INITIAL_TICK_TIME 30 // assume 30ms per tick

class FrequencyCounter {
private:
	uint32 m_aiTickStore[FRQCNT_STORE_SIZE];
	int m_iTickCursor;

	float m_fAvgTimeStep;

public:
	FrequencyCounter();

	float GetAvgTickTimeMs() const;
	void Tick(uint32 timestep);
};

#endif
