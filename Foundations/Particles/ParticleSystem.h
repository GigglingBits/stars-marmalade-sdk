#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include <string>
#include <list>
#include "Renderable.h"
#include "Particle.h"

class ParticleSystem : public Renderable {
private:
	CIwFVec2 m_xPosition;
	
	typedef std::list<Particle*> ParticleList;
	ParticleList m_xParticles;
	
	TextureTemplate m_xTextureTpl;

	std::string m_sCreateSound;
	std::string m_sDestroySound;
	
	CIwFVec2 m_xGravity;
	
	bool m_bIsStarted;
	int m_iNextParticleDueInMs;

	// prarticle properties
	CIwFVec2 m_xParticleSize;
	uint16 m_uiParticleLifetime;
		
public:
	ParticleSystem(const TextureTemplate& tpl, const CIwFVec2& gravity, const std::string& createSound = "", const std::string& destroySound = "");
	~ParticleSystem();
	
	bool IsStarted();
	void Start();
	void Stop();

	void SetPosition(const CIwFVec2& pos);
	
	bool HasParticles();
	
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
