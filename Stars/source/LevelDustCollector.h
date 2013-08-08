#ifndef __LEVELDUSTCOLLECTOR_H__
#define __LEVELDUSTCOLLECTOR_H__

#include "Window.h"
#include "Texture.h"

class LevelDustCollector : public Window {
private:
	CIwRect m_xCollectorShape;

	VertexStreamScreen m_xDustShape;
	Texture* m_pxDust;

	VertexStreamScreen m_xVialShape;
	Texture* m_pxVial;

	float m_fProgress;

public:
	LevelDustCollector();
	virtual ~LevelDustCollector();

	void Initialize();

	void SetProgress(float progress);

protected:
	void UpdateStarShape();

	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);
};

#endif
