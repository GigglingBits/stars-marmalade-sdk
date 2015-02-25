#ifndef __WORLDMENU_H__
#define __WORLDMENU_H__

#include "Page.h"
#include "PageSettings.h"
#include "BackgroundParallax.h"
#include "Button.h"
#include "ButtonEx.h"
#include "NaviPanel.h"
#include "IwList.h"
#include "Body.h"
#include "Camera.h"

class WorldMenu : public Page {
private:
	Camera m_xCamera;

	CIwRect m_xTitlePos;
	std::string m_sTitle;
	
    ButtonEx m_xButtonPlanet;
	Button m_xButtonNext;
	Button m_xButtonPrevious;

	Button m_xButtonBack;
	
	BackgroundParallax m_xBackground;

public:
	WorldMenu();
	virtual ~WorldMenu();

	virtual void Initialize();
	
protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);

private:
	void ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args);
};

#endif
