#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include <string>
#include <list>
#include "Renderable.h"
#include "Particle.h"

class ParticleSystem : public Renderable {
private:
	int m_iNextParticleDueInMs;
	
	typedef std::list<Particle*> ParticleList;
	ParticleList m_xParticles;
	
	TextureTemplate m_xTextureTpl;
	
public:
	ParticleSystem(const TextureTemplate& tpl);
	~ParticleSystem();
	
private:
	void CheckCreateParticles(uint16 elapsedms);
	Particle* CreateParticle();

	void RemoveDeadParticles();
	
private:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
