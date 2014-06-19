#include "s3e.h"
#include "FrameData.h"

#include <map>

FrameData::FrameData() {
	m_xScreenSize = CIwSVec2::g_Zero;
	m_lLastSystemClock = s3eTimerGetMs();
}

const CIwSVec2& FrameData::GetScreensize() const {
	return m_xScreenSize;
}

uint64 FrameData::GetTimeMs() const {
	return m_lLastSystemClock;
}

uint16 FrameData::GetSimulatedDurationMs() const {
	return m_iSimFrameTime;
}

uint16 FrameData::GetRealDurationMs() const {
	return m_iRealFrameTime;
}

float FrameData::GetAvgSimulatedDurationMs() const {
	return m_xSimCounter.GetAvgTickTimeMs();
}

float FrameData::GetAvgRealDurationMs() const {
	return m_xRealCounter.GetAvgTickTimeMs();
}

void FrameData::SetScreenSize(uint32 width, uint32 height) {
	m_xScreenSize.x = width;
	m_xScreenSize.y = height;
}

void FrameData::Tick() {
	// measure time between ticks (real measurement)
	// cap the simulation at 30ms; physics simulations become unstable with larger steps
	Tick((uint16)(s3eTimerGetMs() - m_lLastSystemClock), 30);
}

void FrameData::Tick(uint16 frametime, uint16 simulationcap) {
	m_iRealFrameTime = frametime;
	m_lLastSystemClock += m_iRealFrameTime;
	
	// adjust time for sumulation
	m_iSimFrameTime = std::min<uint16>(m_iRealFrameTime, simulationcap);
	m_iSimFrameTime = std::max<uint16>(m_iSimFrameTime, 1);  // minimal step is 1ms for avoiding numerical problems (eg. div by 0)
	
	// averages
	m_xRealCounter.Tick(m_iRealFrameTime);
	m_xSimCounter.Tick(m_iSimFrameTime);
}
