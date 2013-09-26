#ifndef __WORLDMENU_H__
#define __WORLDMENU_H__

#include "Page.h"
#include "PageSettings.h"
#include "Button.h"
#include "NaviPanel.h"
#include "IwList.h"
#include "Body.h"
#include "Camera.h"

class WorldMenu : public Page {
private:
	Camera m_xCamera;

	PageSettings::WorldId m_eWorld;
	
    Button m_xButtonPlanet;
	Button m_xButtonPlanetName;

	Button m_xButtonNext;
	Button m_xButtonPrevious;

	Button m_xButtonBack;
	NaviPanel m_xNaviPanel;
	
	Texture* m_pxBackground;

public:
	WorldMenu(PageSettings::WorldId world);
	virtual ~WorldMenu();

	virtual void Initialize();

private:
    void ApplyWorld(PageSettings::WorldId world);
	PageSettings::WorldId GetNext(PageSettings::WorldId worldid);
	PageSettings::WorldId GetPrevious(PageSettings::WorldId worldid);
	
protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);

private:
	void ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args);
};

#endif
