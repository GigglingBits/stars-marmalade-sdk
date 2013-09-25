#ifndef __LEVELMENU_H__
#define __LEVELMENU_H__

#include "Page.h"
#include "PageSettings.h"
#include "IwList.h"
#include "Body.h"
#include "Camera.h"
#include "Button.h"
#include "ButtonEx.h"
#include "NaviPanel.h"

#define LVLMENU_BTN_COLS 4
#define LVLMENU_BTN_ROWS 2
#define LVLMENU_BTN_COUNT LVLMENU_BTN_COLS*LVLMENU_BTN_ROWS

class LevelMenu : public Page {
private:
	Camera m_xCamera;
	
	Texture* m_pxBackground;
	ButtonEx* m_apxButtons[LVLMENU_BTN_COUNT];

	Button m_xButtonBack;
	NaviPanel m_xNaviPanel;

	PageSettings::WorldId m_eWorldId;

public:
	LevelMenu(PageSettings::WorldId world);
	virtual ~LevelMenu();

	virtual void Initialize();

protected:
	virtual void OnDoLayout(const CIwSVec2& screensize);
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);

private:
	bool CheckLevelConfiguration(PageSettings::WorldId world, int level);
	void EnableButtons(bool enable);
	
	Button* GetButton(int col, int row);
};

#endif
