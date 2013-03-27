#ifndef __FRAMEDATA_H__ 
#define __FRAMEDATA_H__ 

#include "IwTypes.h"
#include "IwGeom.h"
#include "FrequencyCounter.h"

class FrameData {
private:
	uint64 m_lLastSystemClock;

	CIwSVec2 m_xScreenSize;

	uint16 m_iSimFrameTime;
	uint16 m_iRealFrameTime;

	FrequencyCounter m_xSimCounter;
	FrequencyCounter m_xRealCounter;

public:
	FrameData();

	const CIwSVec2& GetScreensize() const;

	uint64 GetTimeMs() const;

	uint16 GetSimulatedDurationMs() const;
	uint16 GetRealDurationMs() const;

	float GetAvgSimulatedDurationMs() const;
	float GetAvgRealDurationMs() const;

	void SetScreenSize(uint32 width, uint32 height);
	void Tick();
};

#endif
