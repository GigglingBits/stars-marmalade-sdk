#ifndef _STOPWATCH_H_ 
#define _STOPWATCH_H_ 

#include "s3e.h"

class Stopwatch {
private:
	uint64 m_iElapsed;
	uint64 m_iStartedTs;

public:
	Stopwatch();
	void Reset();
	void Start();
	void Stop();
	uint32 GetElapsed();
};

#endif
