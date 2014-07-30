#ifndef __LEVELDUSTVIAL_H__
#define __LEVELDUSTVIAL_H__

#include "Window.h"

class LevelDustVial : public Window {
private:
	static uint32 s_auCommittedColours[];
	static uint32 s_auQueuedColours[];
	
	VertexStreamScreen m_xBackShape;
	Texture* m_pxBack;

	CIwRect m_xContentGeom;
	
	float m_fCommittedDust;
	float m_fCommittedDustInternal;
	VertexStreamScreen m_xCommittedDustShape;
	VertexStreamScreen m_xCommittedDustSurfaceShape;
	Texture* m_pxCommittedDustSurface;
	
	float m_fQueuedDust;
	float m_fQueuedDustInternal;
	VertexStreamScreen m_xQueuedDustShape;
	VertexStreamScreen m_xQueuedDustSurfaceShape;
	Texture* m_pxQueuedDustSurface;
	
public:
	LevelDustVial();
	virtual ~LevelDustVial();

	void Initialize();

	void SetDustAmount(float committed, float queued);
	
private:
	void UpdateAmounts(uint32 timestep);
	float Approximate(float value, float target, uint32 timestep);

	void UpdateVialShapes();
	void UpdateAmountShapes();
	
	void DustDeltaEffect(float amount);
	
protected:
	virtual void OnDoLayout(const CIwSVec2& screensize);
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
