#include "Stopwatch.h"

Stopwatch::Stopwatch() {
	Reset();
}

void Stopwatch::Reset() {
	m_iElapsed = 0;
	m_iStartedTs = 0;
}

void Stopwatch::Start() {
	m_iStartedTs = s3eTimerGetMs();
}

void Stopwatch::Stop() {
	m_iElapsed = s3eTimerGetMs() - m_iStartedTs;
	m_iStartedTs = 0;
}

uint32 Stopwatch::GetElapsed() {
	if (m_iStartedTs == 0) {
		return (uint32)m_iElapsed; 
	}
	return (uint32)(m_iElapsed + (s3eTimerGetMs() - m_iStartedTs)); 	
}
