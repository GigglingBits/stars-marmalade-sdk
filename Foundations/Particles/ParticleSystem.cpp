#include "ParticleSystem.h"
#include "SoundEngine.h"

ParticleSystem::ParticleSystem(const TextureTemplate& tpl, const CIwFVec2& gravity, const std::string& createSound, const std::string& destroySound) :
	m_xTextureTpl(tpl),
	m_iNextParticleDueInMs(0),
	m_bIsStarted(false),
	m_xPosition(0.0f, 0.0f),
	m_xGravity(gravity),
	m_sCreateSound(createSound),
	m_sDestroySound(destroySound),
	m_uiParticleLifetime(1000),
	m_xParticleSize(0.3f, 0.3f) {
}

ParticleSystem::~ParticleSystem() {
	for (ParticleList::iterator i = m_xParticles.begin(); i != m_xParticles.end(); i++) {
		delete *i;
	}
	m_xParticles.clear();
}

bool ParticleSystem::IsStarted() {
	return m_bIsStarted;
}

void ParticleSystem::Start() {
	m_bIsStarted = true;
}

void ParticleSystem::Stop() {
	m_bIsStarted = false;
	
}

bool ParticleSystem::HasParticles() {
	return !m_xParticles.empty();
}

void ParticleSystem::SetParticleSize(const CIwFVec2& size) {
	m_xParticleSize = size;
}

uint16 ParticleSystem::GetParticleLifeTime() {
	return m_uiParticleLifetime;
}

void ParticleSystem::SetParticleLifetime(uint16 lifetime) {
	m_uiParticleLifetime = lifetime;
}

void ParticleSystem::SetPosition(const CIwFVec2& pos) {
	m_xPosition = pos;
}

void ParticleSystem::CreateParticles(uint16 elapsedms) {
	const int interval = 30;
	m_iNextParticleDueInMs -= elapsedms;
	
	bool created = false;
	while (m_iNextParticleDueInMs < 0) {
		m_xParticles.push_back(CreateParticle());
		m_iNextParticleDueInMs += interval;
		created = true;
	}
	
	if (created && !m_sCreateSound.empty()) {
		SoundEngine::GetInstance().PlaySoundEffect(m_sCreateSound);
	}
}

Particle* ParticleSystem::CreateParticle() {
	// veocity
	CIwFVec2 maxstartvelocity(10.0f, 10.0f);
	const int resolution = 1000;
	float x = (float)(rand() % resolution - (resolution / 2)) / resolution;
	float y = (float)(rand() % resolution - (resolution / 2)) / resolution;
	CIwFVec2 velocity(
		maxstartvelocity.x * x,
		maxstartvelocity.y * y);
	
	Particle* particle = new Particle(m_xTextureTpl, m_xParticleSize, m_uiParticleLifetime);
	particle->SetPosition(m_xPosition);
	particle->SetGravity(m_xGravity);
	particle->SetVelocity(velocity);
	particle->SetRederingLayer(Renderer::eRenderingLayerGameBackground);
	return particle;
}

void ParticleSystem::RemoveDeadParticles() {
	bool destroyed = false;
	
	ParticleList::iterator i = m_xParticles.begin();
	while (i != m_xParticles.end()) {
		if ((*i)->IsDead()) {
			destroyed = true;
			delete *i;
			i = m_xParticles.erase(i);
		} else {
			i++;
		}
	}
	
	if (destroyed && !m_sDestroySound.empty()) {
		SoundEngine::GetInstance().PlaySoundEffect(m_sDestroySound);
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
