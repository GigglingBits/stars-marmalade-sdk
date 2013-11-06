#ifndef __PAGEBACKGROUND_H__
#define __PAGEBACKGROUND_H__

#include "Window.h"
#include "s3eGyroscope.h"

class PageBackground : public Window {
private:
	Texture* m_pxBackground;

	s3eGyroscopeData m_xGyro;

public:
	PageBackground();
	~PageBackground();
	
public:
	void Initialize();

private:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	//virtual void OnDoLayout(const CIwSVec2& screensize);
	
private:
	void SetGyroData(const s3eGyroscopeData& data);
	static void GyroscopeCallback(void* sysdata, void* usrdata);
};

#endif
