#ifndef __LEVELDUSTCOLLECTOR_H__
#define __LEVELDUSTCOLLECTOR_H__

#include "Window.h"
#include "Texture.h"

class LevelDustCollector : public Window {
private:
	VertexStreamScreen m_xDustShape;
	Texture* m_pxDust;

	float m_fTargetProgress;
	float m_fDisplayedProgress;
	
	long m_lTotalRollTime;
	long m_lRemainingRollTime;

public:
	LevelDustCollector();
	virtual ~LevelDustCollector();

	void Initialize();

	void SetProgress(float progress, int rolltime = 0);

protected:
	void UpdateDustShape();

	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);
};

#endif
