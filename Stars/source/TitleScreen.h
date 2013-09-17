#ifndef __TITLESCREEN_H__
#define __TITLESCREEN_H__

#include "Page.h"
#include "Button.h"
#include "OptionsPanel.h"
#include "IwList.h"
#include "Body.h"
#include "Camera.h"

class TitleScreen : public Page {
private:
	Camera m_xCamera;

	Texture* m_pxBackground;
	
	Button m_xButtonCredits;
	Button m_xButtonAchievements;

    Button m_xButtonTitle;
	Button m_xButtonTitleName;

	OptionsPanel m_xPanelOptions;

public:
	TitleScreen();
	virtual ~TitleScreen();

	virtual void Initialize();

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);

private:
	void ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args);
	void ChangeButtonState(bool enable, const ButtonPanel& except);
	void ButtonPanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args);
};

#endif
