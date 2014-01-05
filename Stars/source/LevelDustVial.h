#ifndef __LEVELDUSTVIAL_H__
#define __LEVELDUSTVIAL_H__

#include "Window.h"

class LevelDustVial : public Window {
private:
	CIwRect m_xBackGeom;
	CIwRect m_xContentGeom;
	
	float m_fCommittedDust;
	CIwRect m_xCommittedGeom;
	
	float m_fQueuedDust;
	CIwRect m_xQueuedGeom;
	
public:
	LevelDustVial();
	virtual ~LevelDustVial();

	void Initialize();

	void SetDustAmount(float committed, float queued);
	
private:
	void UpdateShapes();
	
protected:
	virtual void OnDoLayout(const CIwSVec2& screensize);
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
