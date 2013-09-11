#include "CameraEffect.h"

CameraEffect::CameraEffect() : m_iTimeRemaining(0), m_iDuration(0), m_xOffeset(0.0f, 0.0f), m_fAmplitude(0.0f) {
}

bool CameraEffect::IsActive() {
	return m_iTimeRemaining > 0;
}

void CameraEffect::StartQuake(float amplitude, uint16 duration) {
	if (!(IsActive() && m_fAmplitude == amplitude)) {
		m_xXOscillator.Set(12.2f, amplitude / -2.0f, amplitude / 2.0f);
		m_xYOscillator.Set(8.1f, amplitude / -2.0f, amplitude / 2.0f);
	}
	
	m_iDuration = duration;
	m_iTimeRemaining = duration;
}

const CIwFVec2& CameraEffect::GetEffectOffset() {
	return m_xOffeset;
}

void CameraEffect::Update(const CIwSVec2& screensize, uint16 timestep) {
	if (!IsActive()) {
		return;
	}

	float modifier = m_iTimeRemaining/(float)m_iDuration;
	
	m_xXOscillator.Update(timestep);
	m_xOffeset.x = m_xXOscillator.GetValue() * modifier;

	m_xYOscillator.Update(timestep);
	m_xOffeset.y = m_xYOscillator.GetValue() * modifier;
	
	m_iTimeRemaining -= timestep;
}
