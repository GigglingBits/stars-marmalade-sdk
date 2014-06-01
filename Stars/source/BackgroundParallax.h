#ifndef __PACKGROUNDPARALLAX_H__
#define __PACKGROUNDPARALLAX_H__

#include "Window.h"
#include "LocationServices.h"

class BackgroundParallax : public Window {
private:
	CIwRect m_xBackgroundRect;
	Texture* m_pxBackground;
	Texture* m_pxBackgroundFar;
	float m_fParallaxCorrection;
	
public:
	BackgroundParallax();
	~BackgroundParallax();
	
public:
	void Initialize();

private:
	virtual void OnDoLayout(const CIwSVec2& screensize);
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	
private:
	void RenderBackground(Renderer& renderer, Texture& texture, const CIwSVec2& orientationoffset);
};

#endif
