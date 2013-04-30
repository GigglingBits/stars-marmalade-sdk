#ifndef __PREAMBLE_H__
#define __PREAMBLE_H__

#include "Page.h"
#include "InputManager.h"
#include "Timer.h"
#include "Camera.h"

class Preamble : public Page {
private:
	Camera m_xCamera;
	CIwSVec2 m_xScreenSize;

	Texture* m_pxBackground;

	std::string m_sText;
	Timer m_xTimer;

public:
	Preamble(const std::string& text);
	virtual ~Preamble();

	virtual void Initialize();

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);

private:
	void TouchEndEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args);
	void TimerElapsedEventHandler(const Timer& sender, const long& data);
};

#endif
