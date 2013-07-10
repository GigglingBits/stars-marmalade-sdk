#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Texture.h"
#include "Renderable.h"

class Particle : public Renderable {
private:
	// world setting
	CIwFVec2 m_xGravity;

	// apprarance
	CIwFVec2 m_xPosition;
	VertexStreamWorld m_xShape;
	Texture m_xTexture;

	// behavior
	CIwFVec2 m_xVelocity;
	int m_iRemainingLifeTimeMs;
	
public:
	Particle(const TextureTemplate& tpl);
	
	bool IsDead();
	
	void SetPosition(const CIwFVec2& point);
	void SetGravity(const CIwFVec2& force);
	void SetVelocity(const CIwFVec2& speed);
	
private:
	void ApplyGravity(const CIwFVec2& g, uint16 simulationtime);
	void ApplyVelocity(const CIwFVec2& v, uint16 simulationtime);
	
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
