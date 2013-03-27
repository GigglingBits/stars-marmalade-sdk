#include "Oscillator.h"
#include <math.h>

bool Oscillator::s_bInitialized = false;
float Oscillator::s_afSinus[OSC_BASE_RESOLUTION];

Oscillator::Oscillator(float frequency, float minval, float maxval) {
	InitCache();
	Set(frequency, minval, maxval);
}

void Oscillator::Set(float frequency, float minval, float maxval) {
	m_fFrequency = frequency;
	m_fMinVal = minval;
	m_fMaxVal = maxval;
	Restart();
}

void Oscillator::Restart() {
	m_iCursor = 0;
	UpdateCurentValue();
}

void Oscillator::InitCache() {
	if (!s_bInitialized) {
		double step = 2.0 * M_PI / (double)OSC_BASE_RESOLUTION;
		for (int i = 0; i < OSC_BASE_RESOLUTION; i++) {
			s_afSinus[i] = (float)sin(step * (double)i);
		}
		s_bInitialized = true;
	}
}

float Oscillator::GetValue() {
	return m_fCurrentValue;
}

void Oscillator::UpdateCurentValue() {
	float range = (m_fMaxVal - m_fMinVal) / 2.0f;
	m_fCurrentValue = range + m_fMinVal + (s_afSinus[m_iCursor] * range);
}

void Oscillator::Update(uint16 timestep) {
	uint16 increment = (OSC_BASE_RESOLUTION * timestep) / ((uint16)(1000 / m_fFrequency));
	m_iCursor = (m_iCursor + increment) % OSC_BASE_RESOLUTION;
	UpdateCurentValue();
}
