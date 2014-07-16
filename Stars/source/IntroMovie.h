#ifndef __INTROMOVIE_H__
#define __INTROMOVIE_H__

#include "Page.h"
#include "PageSettings.h"
#include "InputManager.h"
#include "Timer.h"
#include "Camera.h"
#include "VideoView.h"

class IntroMovie : public Page {
private:
	Camera m_xCamera;
	VideoView m_xVideoView;
	
public:
	IntroMovie(const std::string& videofile);
	virtual ~IntroMovie();
	
	virtual void Initialize();
	
protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);
	
private:
	MediaView& SelectMediaView(const std::string& file);
	
	void TouchEndEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args);
	void MediaFinishedEventHandler(const MediaView& sender, const int& args);
};

#endif