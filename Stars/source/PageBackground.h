#ifndef __PAGEBACKGROUND_H__
#define __PAGEBACKGROUND_H__

#include "Window.h"
#include "LocationServices.h"

class PageBackground : public Window {
private:
	Texture* m_pxBackground;
	float m_fParallaxCorrection;
	
public:
	PageBackground();
	~PageBackground();
	
public:
	void Initialize();

private:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
