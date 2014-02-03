#ifndef __WORLDMENU_H__
#define __WORLDMENU_H__

#include "Page.h"
#include "PageSettings.h"
#include "PageBackground.h"
#include "Button.h"
#include "NaviPanel.h"
#include "IwList.h"
#include "Body.h"
#include "Camera.h"

class WorldMenu : public Page {
private:
	Camera m_xCamera;

	LevelIndexer::WorldId m_eWorld;
	
	CIwRect m_xTitlePos;
	CIwRect m_xTitleShadowPos;
	std::string m_sTitle;
	
    Button m_xButtonPlanet;

	Button m_xButtonNext;
	Button m_xButtonPrevious;

	Button m_xButtonBack;
	NaviPanel m_xNaviPanel;
	
	PageBackground m_xBackground;

public:
	WorldMenu(LevelIndexer::WorldId world);
	virtual ~WorldMenu();

	virtual void Initialize();

private:
    void ApplyWorld(LevelIndexer::WorldId world);
	LevelIndexer::WorldId GetNext(LevelIndexer::WorldId worldid);
	LevelIndexer::WorldId GetPrevious(LevelIndexer::WorldId worldid);
	
	
	
protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);

private:
	void ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args);
};

#endif
