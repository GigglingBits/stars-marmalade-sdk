#ifndef __WORLDMENU_H__
#define __WORLDMENU_H__

#include "Page.h"
#include "PageSettings.h"
#include "Button.h"
#include "OptionsPanel.h"
#include "IwList.h"
#include "Body.h"
#include "Camera.h"

class WorldMenu : public Page {
private:
	Camera m_xCamera;

	Texture* m_pxBackground;
	
    Button m_xButtonPlanet;
	Button m_xButtonPlanetName;

	Button m_xButtonNext;
	Button m_xButtonPrevious;

	Button m_xButtonBack;
	
    PageSettings::WorldId m_eWorld;
    
public:
	WorldMenu(PageSettings::WorldId world);
	virtual ~WorldMenu();

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
};

#endif
