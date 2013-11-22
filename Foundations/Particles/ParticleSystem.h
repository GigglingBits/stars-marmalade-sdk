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
	
public:
	ParticleSystem(const TextureTemplate& tpl, const CIwFVec2& gravity, const std::string& createSound = "", const std::string& destroySound = "");
	~ParticleSystem();
	
	void Start();
	void Stop();

	void SetPosition(const CIwFVec2& pos);
	
private:
	void CreateParticles(uint16 elapsedms);
	Particle* CreateParticle();

	void RemoveDeadParticles();
	
private:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
