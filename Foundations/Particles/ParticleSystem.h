#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include <string>
#include <list>
#include "Renderable.h"
#include "Particle.h"

class ParticleSystem : public Renderable {
private:
	// internal helpers
	typedef std::list<Particle*> ParticleList;
	ParticleList m_xParticles;
	int m_iNextParticleDueInMs;
	
	// system properties
	uint64 m_ullSystemTimeLeft;

	CIwFVec2 m_xPosition;
	CIwFVec2 m_xGravity;
	uint16 m_uiBirthRate;

	std::string m_sCreateSound;
	std::string m_sDestroySound;
		
	// particle properties
	TextureTemplate m_xTextureTpl;

	CIwFVec2 m_xParticleSpeed;
	CIwFVec2 m_xParticleSize;
	uint16 m_uiParticleLifetime;
		
public:
	ParticleSystem(const TextureTemplate& tpl, const CIwFVec2& gravity = CIwFVec2::g_Zero, const std::string& createSound = "", const std::string& destroySound = "");
	~ParticleSystem();
	
	bool IsStarted();
	void Start(uint64 durationms = UINT64_MAX);
	void Stop();
	
	bool HasParticles();

public:
	void SetPosition(const CIwFVec2& pos);
	void SetBirthRate(uint16 particlespersecond);
	
public:
	void SetParticleSpeed(const CIwFVec2& v);
	void SetParticleSize(const CIwFVec2& size);

	uint16 GetParticleLifeTime();
	void SetParticleLifetime(uint16 lifetime);
	
private:
	void CreateParticles(uint16 elapsedms);
	Particle* CreateParticle();

	void RemoveDeadParticles();
	
private:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
