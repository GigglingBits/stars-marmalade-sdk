#include "BodyHealth.h"

BodyHealth::BodyHealth() {
	m_iHealth = 100;
	m_fResilience = 1.0;
}

void BodyHealth::SetResilience(float resi) {
	m_fResilience = resi;
}

void BodyHealth::RenewLife() {
	m_iHealth = 100;
}

void BodyHealth::Kill() {
	m_iHealth = 0;
}

void BodyHealth::Injure(float impulse) {
	if (impulse > 0.5f) {
		m_iHealth -= (int)(impulse * (1.0f - m_fResilience) * 2.0f);
	}
}

bool BodyHealth::IsDead() {
	return m_iHealth <= 0;
}

int BodyHealth::GetHealthValue() {
	return m_iHealth;
}
