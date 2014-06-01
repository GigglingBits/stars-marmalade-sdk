#ifndef __LEVELMENU_H__
#define __LEVELMENU_H__

#include "Page.h"
#include "PageSettings.h"
#include "IwList.h"
#include "Camera.h"
#include "LevelMenuButton.h"
#include "NaviPanel.h"
#include "BackgroundParallax.h"

#define LVLMENU_BTN_COLS 3
#define LVLMENU_BTN_ROWS 2
#define LVLMENU_BTN_GROUP_COUNT 3
#define LVLMENU_BTN_COUNT_PER_GROUP (LVLMENU_BTN_COLS*LVLMENU_BTN_ROWS)

class LevelMenu : public Page {
private:
	Camera m_xCamera;
	
	LevelMenuButton* m_apxButtons[LVLMENU_BTN_COUNT_PER_GROUP];
	Button m_xButtonNext;
	Button m_xButtonPrevious;
	Button m_xButtonBack;
	
	NaviPanel m_xNaviPanel;

	LevelIterator::WorldId m_eWorldId;
	int m_iGroupId;
	int m_iFirstUnachievedLevel;
	
	BackgroundParallax m_xBackground;

public:
	LevelMenu(LevelIterator::WorldId world);
	virtual ~LevelMenu();

	virtual void Initialize();

protected:
	virtual void OnDoLayout(const CIwSVec2& screensize);
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);

private:
	std::string GetLevelKey(LevelIterator::WorldId world, int level);
	bool CheckLevelExists(LevelIterator::WorldId world, int level);
	int GetFirstUnachievedLevel();
	int GetAchievedStars(LevelIterator::WorldId world, int level);
	
	void EnableButtons(bool enable);
	
	Button* GetButton(int col, int row);

	void ApplyGroup(int groupid);
	int GetNext(int groupid);
	int GetPrevious(int groupid);
		
private:
	void ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args);
};

#endif
