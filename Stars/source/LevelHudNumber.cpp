#include <sstream>
#include "LevelHudNumber.h"
#include "Debug.h"
#include "FactoryManager.h"

LevelHudNumber::LevelHudNumber() : m_lInternalNumber(0), m_lDisplayedNumber(0), m_lTotalRollTime(0), m_lRemainingRollTime(0) {
	UpdateText();
}

void LevelHudNumber::SetNumber(long number) {
	m_lInternalNumber = number;
	m_lRemainingRollTime = 0;
}

void LevelHudNumber::SetRollingNumber(long number, int milliseconds) {
	if (m_lInternalNumber != number) {
		m_lInternalNumber = number;
		m_lTotalRollTime = milliseconds;
		m_lRemainingRollTime = m_lTotalRollTime;
	}
}

void LevelHudNumber::UpdateText() {
	std::ostringstream oss;
	oss << m_lDisplayedNumber;
	SetText(oss.str());
}

void LevelHudNumber::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	LevelHudText::OnUpdate(frame);
	
	// needs rolling?
	if (m_lDisplayedNumber != m_lInternalNumber) {
		// time exceeded?
		if (m_lRemainingRollTime <= 0) {
			m_lDisplayedNumber = m_lInternalNumber;
		} else {
			// roll the number
			long error = m_lInternalNumber - m_lDisplayedNumber;
			long correction = error * (m_lTotalRollTime - m_lRemainingRollTime) / m_lTotalRollTime;
			m_lDisplayedNumber = m_lDisplayedNumber + correction;
		}
		
		m_lRemainingRollTime -= frame.GetSimulatedDurationMs();
		UpdateText();
	}
}
