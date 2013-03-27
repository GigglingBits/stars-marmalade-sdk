#include "FrequencyCounter.h"

FrequencyCounter::FrequencyCounter() {
	m_fAvgTimeStep = FRQCNT_INITIAL_TICK_TIME;
	for (int i = 0; i < FRQCNT_STORE_SIZE; i++) {
		m_aiTickStore[i] = (uint32)m_fAvgTimeStep;
	}
	m_iTickCursor = 0;
}

float FrequencyCounter::GetAvgTickTimeMs() const {
	return m_fAvgTimeStep;
}

void FrequencyCounter::Tick(uint32 timestep) {
	// record new tick
	m_aiTickStore[m_iTickCursor++] = timestep;
	if (m_iTickCursor == FRQCNT_STORE_SIZE) {
		m_iTickCursor = 0;
	}

	// update average
	uint32 sum = 0;
	for (int i = 0; i < FRQCNT_STORE_SIZE; i++) {
		sum += m_aiTickStore[i];
	}
	m_fAvgTimeStep = sum / (float)FRQCNT_STORE_SIZE;
}
