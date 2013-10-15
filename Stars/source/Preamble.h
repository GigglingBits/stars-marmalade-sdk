#ifndef __PREAMBLE_H__
#define __PREAMBLE_H__

#include "Page.h"
#include "PageSettings.h"
#include "InputManager.h"
#include "Timer.h"
#include "Camera.h"
#include "WebView.h"
#include "VideoView.h"
#include "TextureView.h"

class Preamble : public Page {
private:
	Camera m_xCamera;

	std::string m_sText;
	CIwRect m_xTextPosition;

	std::string m_sTextureId;
	std::string m_sMediaFile;
	
	MediaView* m_pxMediaView;
	Texture* m_pxBackground;
	
public:
	Preamble(const std::string& text, const std::string& textureid, const std::string& mediafile);
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
