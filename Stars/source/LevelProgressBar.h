#ifndef __LEVELPROGRESSBAR_H__
#define __LEVELPROGRESSBAR_H__

#include "Window.h"
#include "Texture.h"

class LevelProgressBar : public Window {
private:
	VertexStreamScreen m_xBackgroundShape;
	Texture* m_pxBackground;

	VertexStreamScreen m_xStarShape;
	Texture* m_pxStar;

	float m_fProgress;

public:
	LevelProgressBar();
	virtual ~LevelProgressBar();

	void Initialize();

	void SetProgress(float progress);

protected:
	void UpdateStarShape();

	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);
};

#endif
