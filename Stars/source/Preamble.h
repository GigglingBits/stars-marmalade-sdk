#ifndef __PREAMBLE_H__
#define __PREAMBLE_H__

#include "Page.h"
#include "PageSettings.h"
#include "InputManager.h"
#include "Timer.h"
#include "Camera.h"
#include "WebView.h"
#include "VideoView.h"

class Preamble : public Page {
private:
	class NullMediaView : public MediaView {
	public:
		virtual bool IsShowing();
		virtual void Show(const CIwVec2& pos, const CIwVec2& size);
		virtual void Hide();
	};

private:
	Camera m_xCamera;

	std::string m_sText;

	Texture* m_pxBackground;

	MediaView* m_pxMediaView;
	NullMediaView m_xNull;
	WebView m_xWeb;
	VideoView m_xVideo;

public:
	Preamble(const std::string& text, const std::string& file);
	virtual ~Preamble();

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
