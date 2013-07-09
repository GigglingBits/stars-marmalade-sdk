#include "Particle.h"

Particle::Particle(const TextureTemplate& tpl, const CIwFVec2& position, const CIwFVec2& velocity) : m_xTexture(tpl), m_xGravity(0.0f, -9.81f), m_iRemainingLifeTimeMs(3000) {
	m_xPosition = position;
	m_xVelocity = velocity;
	m_xShape.SetRect(m_xPosition.x, m_xPosition.y, 0.5f, 0.5f);
}

bool Particle::IsDead() {
	return m_iRemainingLifeTimeMs <= 0;
}

void Particle::OnUpdate(const FrameData& frame) {
	if (m_iRemainingLifeTimeMs > 0) {
		m_iRemainingLifeTimeMs -= frame.GetSimulatedDurationMs();
	}

	int16 simtime = frame.GetSimulatedDurationMs();
	m_xTexture.Update(simtime);

	ApplyGravity(m_xGravity, simtime);
	ApplyVelocity(m_xVelocity, simtime);
}

void Particle::OnRender(Renderer& renderer, const FrameData& frame) {
	m_xShape.SetRect(m_xPosition.x, m_xPosition.y, 0.5f, 0.5f);
	renderer.Draw(m_xShape, m_xTexture);
}

void Particle::ApplyGravity(const CIwFVec2& g, uint16 simulationtime) {
	CIwFVec2 dg = g * (float)simulationtime / 1000.0f;
	m_xVelocity += dg;
}

void Particle::ApplyVelocity(const CIwFVec2& v, uint16 simulationtime) {
	CIwFVec2 d = v * (float)simulationtime / 1000.0f;
	m_xPosition += d;
}
