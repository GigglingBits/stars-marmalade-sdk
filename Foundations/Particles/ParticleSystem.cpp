#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(const TextureTemplate& tpl, const CIwFVec2& gravity) : m_xTextureTpl(tpl), m_iNextParticleDueInMs(0), m_bIsStarted(false), m_xPosition(0.0f, 0.0f), m_xGravity(gravity) {
}

ParticleSystem::~ParticleSystem() {
	for (ParticleList::iterator i = m_xParticles.begin(); i != m_xParticles.end(); i++) {
		delete *i;
	}
	m_xParticles.clear();
}

void ParticleSystem::Start() {
	m_bIsStarted = true;
}

void ParticleSystem::Stop() {
	m_bIsStarted = false;
	
}

void ParticleSystem::SetPosition(const CIwFVec2& pos) {
	m_xPosition = pos;
}

void ParticleSystem::CreateParticles(uint16 elapsedms) {
	const int interval = 50;
	
	m_iNextParticleDueInMs -= elapsedms;
	while (m_iNextParticleDueInMs < 0) {
		m_xParticles.push_back(CreateParticle());
		m_iNextParticleDueInMs += interval;
	}
}

Particle* ParticleSystem::CreateParticle() {
	CIwFVec2 maxstartvelocity(10.0f, 10.0f);
	
	// veocity
	const int resolution = 1000;
	float x = (float)(rand() % resolution - (resolution / 2)) / resolution;
	float y = (float)(rand() % resolution - (resolution / 2)) / resolution;
	CIwFVec2 velocity(
		maxstartvelocity.x * x,
		maxstartvelocity.y * y);
	
	Particle* particle = new Particle(m_xTextureTpl);
	particle->SetPosition(m_xPosition);
	particle->SetGravity(m_xGravity);
	particle->SetVelocity(velocity);
	particle->SetRederingLayer(Renderer::eRenderingLayerBackgroundGameObjects);
	return particle;
}

void ParticleSystem::RemoveDeadParticles() {
	ParticleList::iterator i = m_xParticles.begin();
	while (i != m_xParticles.end()) {
		if ((*i)->IsDead()) {
			delete *i;
			i = m_xParticles.erase(i);
		} else {
			i++;
		}
	}
}

void ParticleSystem::OnUpdate(const FrameData& frame) {
	RemoveDeadParticles();
	if (m_bIsStarted) {
		CreateParticles(frame.GetSimulatedDurationMs());
	}
	
	for (ParticleList::iterator i = m_xParticles.begin(); i != m_xParticles.end(); i++) {
		if (*i) {
			(*i)->Update(frame);
		}
	}
}

void ParticleSystem::OnRender(Renderer& renderer, const FrameData& frame) {
	for (ParticleList::iterator i = m_xParticles.begin(); i != m_xParticles.end(); i++) {
		if (*i) {
			(*i)->Render(renderer, frame);
		}
	}
}
