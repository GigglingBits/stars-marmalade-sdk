#include "Particle.h"

Particle::Particle(const TextureTemplate& tpl, const CIwFVec2& size, uint16 lifetime) : m_xTexture(tpl), m_xPosition(0.0f, 0.0f), m_xGravity(0.0f, -9.81f), m_iRemainingLifeTimeMs(lifetime) {
	m_xShape.SetRect(m_xPosition.x, m_xPosition.y, m_xSize.x, m_xSize.y);
}

bool Particle::IsDead() {
	return m_iRemainingLifeTimeMs <= 0;
}

void Particle::SetPosition(const CIwFVec2& point) {
	m_xPosition = point;
}

void Particle::SetGravity(const CIwFVec2& force) {
	m_xGravity = force;
}

void Particle::SetVelocity(const CIwFVec2& speed) {
	m_xVelocity = speed;
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
	m_xShape.SetRect(m_xPosition.x, m_xPosition.y, m_xSize.x, m_xSize.y);
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
