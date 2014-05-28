#ifndef __TITLESCREEN_H__
#define __TITLESCREEN_H__

#include "Page.h"
#include "PageBackground.h"
#include "Button.h"
#include "IwList.h"
#include "Body.h"
#include "Camera.h"

#include "AnimTexture.h"

class TitleScreen : public Page {
private:
	Camera m_xCamera;

	PageBackground m_xBackground;
	Button m_xButtonTitle;
	Button m_xButtonFacebook;

	bool m_bHasFacebookButton;
	
public:
	TitleScreen();
	virtual ~TitleScreen();

	virtual void Initialize();
	
protected:
	virtual void OnDoLayout(const CIwSVec2& screensize);
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);

private:
	void ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args);
};

#endif
