#include <sstream>
#include "LevelHudNumber.h"
#include "Debug.h"
#include "FactoryManager.h"

LevelHudNumber::LevelHudNumber() : m_lTargetNumber(0), m_lDisplayedNumber(0), m_lTotalRollTime(0), m_lRemainingRollTime(0) {
	UpdateText();
}

void LevelHudNumber::SetNumber(long number, int rolltime) {
	if (m_lTargetNumber == number) {
		return;
	}
	
	m_lTargetNumber = number;
	m_lTotalRollTime = rolltime;
	m_lRemainingRollTime = m_lTotalRollTime;
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
	if (m_lDisplayedNumber != m_lTargetNumber) {
		// time exceeded?
		if (m_lRemainingRollTime <= 0) {
			m_lDisplayedNumber = m_lTargetNumber;
		} else {
			// roll the number
			long error = m_lTargetNumber - m_lDisplayedNumber;
			long correction = error * (m_lTotalRollTime - m_lRemainingRollTime) / m_lTotalRollTime;
			m_lDisplayedNumber += correction;
		}
		
		m_lRemainingRollTime -= frame.GetSimulatedDurationMs();
		UpdateText();
	}
}
