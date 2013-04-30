#ifndef __LEVELMENU_H__
#define __LEVELMENU_H__

#include "Page.h"
#include "IwList.h"
#include "Body.h"
#include "Camera.h"
#include "Button.h"
#include "OptionsPanel.h"

#define LVLMENU_BTN_COLS 5
#define LVLMENU_BTN_ROWS 2
#define LVLMENU_BTN_COUNT LVLMENU_BTN_COLS*LVLMENU_BTN_ROWS

class LevelMenu : public Page {
private:
	Camera m_xCamera;
	
	Texture* m_pxBackground;
	Button* m_apxButtons[LVLMENU_BTN_COUNT];

	Button m_xButtonAchievements;
	OptionsPanel m_xPanelOptions;

public:
	LevelMenu();
	virtual ~LevelMenu();

	virtual void Initialize();

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);

private:
	Button* GetButton(int col, int row);
	void ChangeState(bool enable, const ButtonPanel& except);
	void ButtonPanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args);
};

#endif
