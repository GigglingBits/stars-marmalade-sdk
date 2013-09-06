#ifndef __TITLEMENU_H__
#define __TITLEMENU_H__

#include "Page.h"
#include "PageSettings.h"
#include "Button.h"
#include "OptionsPanel.h"
#include "IwList.h"
#include "Body.h"
#include "Camera.h"

class TitleMenu : public Page {
private:
	Camera m_xCamera;

	Texture* m_pxBackground;
	
	Button m_xButtonCredits;
	Button m_xButtonAchievements;

    Button m_xButtonPlanet;
	Button m_xButtonPlanetName;

	Button m_xButtonNext;
	Button m_xButtonPrevious;

	OptionsPanel m_xPanelOptions;

    PageSettings::WorldId m_eWorld;
    
public:
	TitleMenu(PageSettings::WorldId world);
	virtual ~TitleMenu();

	virtual void Initialize();

private:
    void SetWorld(PageSettings::WorldId world);
    
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
