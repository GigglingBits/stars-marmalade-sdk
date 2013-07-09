#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(const TextureTemplate& tpl) : m_xTextureTpl(tpl), m_iNextParticleDueInMs(0) {
}

ParticleSystem::~ParticleSystem() {
	for (ParticleList::iterator i = m_xParticles.begin(); i != m_xParticles.end(); i++) {
		delete *i;
	}
	m_xParticles.clear();
}

void ParticleSystem::CheckCreateParticles(uint16 elapsedms) {
	const int interval = 40;
	
	m_iNextParticleDueInMs -= elapsedms;
	while (m_iNextParticleDueInMs < 0) {
		m_xParticles.push_back(CreateParticle());
		m_iNextParticleDueInMs += interval;
	}
}

Particle* ParticleSystem::CreateParticle() {
	CIwFVec2 pos(5.0f, 5.0f);
	CIwFVec2 maxstartvelocity(20.0f, 20.0f);
	
	// veocity
	const int resolution = 1000;
	float x = (float)(rand() % resolution - (resolution / 2)) / resolution;
	float y = (float)(rand() % resolution - (resolution / 2)) / resolution;
	CIwFVec2 velocity(
		maxstartvelocity.x * x,
		maxstartvelocity.y * y);
	
	Particle* particle = new Particle(m_xTextureTpl, pos, velocity);
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
	CheckCreateParticles(frame.GetSimulatedDurationMs());
	
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
