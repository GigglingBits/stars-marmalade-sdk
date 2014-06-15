#include <sstream>
#include "HudNumber.h"
#include "Debug.h"
#include "FactoryManager.h"

HudNumber::HudNumber() : m_lTargetNumber(0), m_lLastTargetNumber(0), m_lTotalRollTime(0), m_lRemainingRollTime(0) {
	UpdateText();
}

void HudNumber::SetNumber(long number, int rolltime) {
	if (m_lTargetNumber == number) {
		return;
	}
	
	m_lLastTargetNumber = m_lTargetNumber;
	m_lTargetNumber = number;

	m_lTotalRollTime = rolltime;
	m_lRemainingRollTime = m_lTotalRollTime;
}

void HudNumber::UpdateText() {
	// interpolate
	float progress = m_lRemainingRollTime / (float) m_lTotalRollTime;
	long number;
	if (progress < 99.9f) {
		number = (1 - progress) * m_lTargetNumber + progress * m_lLastTargetNumber;
	} else {
		number = m_lTargetNumber;
	}
		
	// display
	std::ostringstream oss;
	oss << number;
	SetText(oss.str());
}

void HudNumber::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	HudText::OnUpdate(frame);
	
	// needs rolling?
	if (m_lLastTargetNumber != m_lTargetNumber) {
		// roll the number
		m_lRemainingRollTime -= frame.GetSimulatedDurationMs();

		// time exceeded?
		if (m_lRemainingRollTime <= 0) {
			m_lLastTargetNumber = m_lTargetNumber;
		}

		UpdateText();
	}
}
