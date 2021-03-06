#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "TextureFramed.h"
#include "Renderable.h"

class Particle : public Renderable {
private:
	// world setting
	CIwFVec2 m_xGravity;

	// apprarance
	CIwFVec2 m_xPosition;
	CIwFVec2 m_xSize;
	VertexStreamWorld m_xShape;
	TextureFramed m_xTexture;

	// behavior
	CIwFVec2 m_xVelocity;
	int m_iRemainingLifeTimeMs;
	
public:
	Particle(const TextureTemplate& tpl, const CIwFVec2& size, uint16 lifetime);
	
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
