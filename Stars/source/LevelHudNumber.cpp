#include "LevelHudNumber.h"
#include "Debug.h"
#include "FactoryManager.h"

LevelHudNumber::LevelHudNumber() : m_lInternalNumber(0), m_lDisplayedNumber(0), m_lRemainingRollTime(0) {
	UpdateText();
}

void LevelHudNumber::SetNumber(long number) {
	m_lInternalNumber = number;
	m_lRemainingRollTime = 0;
}

void LevelHudNumber::SetRollingNumber(long number) {
	if (m_lInternalNumber != number) {
		m_lInternalNumber = number;
		m_lRemainingRollTime = NUMBER_ROLL_TIME;
	}
}

void LevelHudNumber::UpdateText() {
	const int bufsize = 32;
	char buf[bufsize];
	snprintf(buf, bufsize, "%li", m_lDisplayedNumber);
	SetText(buf);
}

void LevelHudNumber::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	// needs rolling?
	if (m_lDisplayedNumber != m_lInternalNumber) {
		// time exceeded?
		if (m_lRemainingRollTime <= 0) {
			m_lDisplayedNumber = m_lInternalNumber;
		} else {
			// roll the number
			long error = m_lInternalNumber - m_lDisplayedNumber;
			long correction = error * (NUMBER_ROLL_TIME - m_lRemainingRollTime) / NUMBER_ROLL_TIME;
			m_lDisplayedNumber = m_lDisplayedNumber + correction;
		}
		
		m_lRemainingRollTime -= frame.GetSimulatedDurationMs();
		UpdateText();
	}
}
