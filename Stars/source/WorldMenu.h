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

	LevelIterator::WorldId m_eWorld;
	
	CIwRect m_xTitlePos;
	std::string m_sTitle;
	
    ButtonEx m_xButtonPlanet;
	Texture* m_apxButtonPlanetTextures[LevelIterator::eWorldIdMax];
	
	Button m_xButtonNext;
	Button m_xButtonPrevious;

	Button m_xButtonBack;
	NaviPanel m_xNaviPanel;
	
	BackgroundParallax m_xBackground;

public:
	WorldMenu(LevelIterator::WorldId world);
	virtual ~WorldMenu();

	virtual void Initialize();

private:
	LevelIterator::WorldId GetNext(LevelIterator::WorldId world);
	LevelIterator::WorldId GetPrevious(LevelIterator::WorldId world);

    void ApplyWorld(LevelIterator::WorldId world);
	bool CheckWorldOpen(LevelIterator::WorldId wold);
	
protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);

private:
	void ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args);
};

#endif
