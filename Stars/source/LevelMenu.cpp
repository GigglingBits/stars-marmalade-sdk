#include <sstream>

#include "LevelMenu.h"
#include "FactoryManager.h"
#include "Debug.h"
#include "Configuration.h"
#include "Debug.h"

#define NO_BUTTON -1

LevelMenu::LevelMenu(PageSettings::WorldId world) :
	Page("menu.group", Configuration::GetInstance().IntroSong),
	m_eWorldId(world),
	m_iGroupId(0),
    m_xButtonNext(eButtonCommandIdNone, s3eKeyAbsRight),
    m_xButtonPrevious(eButtonCommandIdNone, s3eKeyAbsLeft),
    m_xButtonBack(eButtonCommandIdOpenWorldMenu, s3eKeyAbsGameD) {

	memset(m_apxButtons, 0, sizeof(m_apxButtons));

	m_xButtonNext.PressedEvent.AddListener<LevelMenu>(this, &LevelMenu::ButtonPressedEventHandler);
	m_xButtonPrevious.PressedEvent.AddListener<LevelMenu>(this, &LevelMenu::ButtonPressedEventHandler);
}

LevelMenu::~LevelMenu() {
	m_xButtonNext.PressedEvent.RemoveListener<LevelMenu>(this, &LevelMenu::ButtonPressedEventHandler);
	m_xButtonPrevious.PressedEvent.RemoveListener<LevelMenu>(this, &LevelMenu::ButtonPressedEventHandler);

	for (int i = 0; i < LVLMENU_BTN_COUNT_PER_GROUP; i++) {
		if (m_apxButtons[i]) {
			delete m_apxButtons[i];
		}
	}
}

void LevelMenu::Initialize() {
	IW_CALLSTACK_SELF;
	m_xButtonNext.SetTexture(FactoryManager::GetTextureFactory().Create("button_arrow_right"));
	m_xButtonPrevious.SetTexture(FactoryManager::GetTextureFactory().Create("button_arrow_left"));

	for (int i = 0; i < LVLMENU_BTN_COUNT_PER_GROUP; i++) {
		IwAssertMsg(MYAPP, !m_apxButtons[i], ("Button %i already exists!", i));
		m_apxButtons[i] = new ButtonEx(eButtonCommandIdOpenLevel, s3eKeyFirst, -1);
		m_apxButtons[i]->SetTexture(FactoryManager::GetTextureFactory().Create("button_level"));
	}

	m_xNaviPanel.Initialize();
	for (int i = 0; i < LVLMENU_BTN_GROUP_COUNT; i++) {
		m_xNaviPanel.AddButton("navipanel", i);
	}

	m_xButtonBack.SetTexture(FactoryManager::GetTextureFactory().Create("button_quit"));

	ApplyGroup(m_iGroupId);

	// update background
	PageSettings ps;
	ps.SetWorld(m_eWorldId);
	SetBackground(
		ps.GetWorldColours().LowerLeft,
		ps.GetWorldColours().LowerRight,
		ps.GetWorldColours().UpperRight,
		ps.GetWorldColours().UpperLeft);
	m_xBackground.Initialize();
}

void LevelMenu::OnDoLayout(const CIwSVec2& screensize) {
	int extents = GetScreenExtents();
	int space = extents / 12;
	CIwSVec2 screencenter(screensize.x / 2, screensize.y / 2);
	
	CIwRect menuarea;
    menuarea.w = (int16)(extents / 1.3f);
    menuarea.h = (int16)(extents / 1.8f);
    menuarea.x = screencenter.x - (menuarea.w / 2);
	menuarea.y = screencenter.y - (menuarea.h / 2);
	
	// level buttons
	CIwRect button(0, 0,
		((menuarea.w + space) / LVLMENU_BTN_COLS) - space,
		((menuarea.h + space) / LVLMENU_BTN_ROWS) - space);
	
	for (int row = 0; row < LVLMENU_BTN_ROWS; row++) {
		for (int col = 0; col < LVLMENU_BTN_COLS; col++) {
			if (Button* p = GetButton(col, row)) {
				button.x = menuarea.x + col * (button.w + space);
				button.y = menuarea.y + row * (button.h + space);
				p->SetPosition(button);
			}
		}
	}
	EnableButtons(true);
	
    // navigation buttons
    button.w = extents / 5;
    button.h = extents / 5;
    button.x = screencenter.x - (extents * 5 / 8);
	button.y = screencenter.y - (button.h / 2);
	m_xButtonPrevious.SetPosition(button);
    
	button.x = screencenter.x + (extents * 5 / 8) - button.w;
	m_xButtonNext.SetPosition(button);
	
	// navi panel
	int navidosize = extents / 15;
	button.w = LVLMENU_BTN_GROUP_COUNT * navidosize;
    button.h = navidosize;
    button.x = screencenter.x - (button.w / 2);
	button.y = screencenter.y + (extents / 3);
	m_xNaviPanel.SetPosition(button);

    // back button
	uint32 btnsize = extents / 10;
	uint32 btnmargin = extents / 30;
	m_xButtonBack.SetPosition(
		CIwRect(screensize.x - (btnsize + btnmargin),
			btnmargin, btnsize, btnsize));
}

void LevelMenu::OnUpdate(const FrameData& frame) {
	// update level buttons
	for (int i = 0; i < LVLMENU_BTN_COUNT_PER_GROUP; i++) {
		if (m_apxButtons[i]) {
			m_apxButtons[i]->Update(frame);
		}
	}

	// update other buttons
	m_xButtonPrevious.Update(frame);
	m_xButtonNext.Update(frame);
	m_xNaviPanel.Update(frame);
	m_xButtonBack.Update(frame);

	m_xBackground.Update(frame);
}

void LevelMenu::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.SetViewport(m_xCamera.GetViewport());
	m_xBackground.Render(renderer, frame);
    
    // level buttons
	for (int i = 0; i < LVLMENU_BTN_COUNT_PER_GROUP; i++) {
		if (m_apxButtons[i]) {
			m_apxButtons[i]->Render(renderer, frame);
		}
	}

	// other buttons
	m_xButtonPrevious.Render(renderer, frame);
	m_xButtonNext.Render(renderer, frame);
	m_xNaviPanel.Render(renderer, frame);
	m_xButtonBack.Render(renderer, frame);
}

Button* LevelMenu::GetButton(int col, int row) {
	return m_apxButtons[col + (row * LVLMENU_BTN_COLS)];
}

void LevelMenu::EnableButtons(bool enable) {
	for (int i = 0; i < LVLMENU_BTN_COUNT_PER_GROUP; i++) {
		if (m_apxButtons[i]) {
			if (enable) {
				m_apxButtons[i]->SetHideWhenDisabled(false);
				m_apxButtons[i]->SetEnabled(CheckLevelConfiguration(m_eWorldId, m_apxButtons[i]->GetUserData()));
			} else {
				m_apxButtons[i]->SetHideWhenDisabled(true);
				m_apxButtons[i]->SetEnabled(false);
			}
		}
	}
}

bool LevelMenu::CheckLevelConfiguration(PageSettings::WorldId world, int level) {
	PageSettings settings;
	settings.SetWorld(world);
	settings.SetLevel(level);
	return FactoryManager::GetLevelFactory().ConfigExists(settings.GetLevelKey());
}

void LevelMenu::ApplyGroup(int groupid) {
	// navigation panel
	m_xNaviPanel.ActivateButton(groupid);
	
	// update the meaning of the level buttons
	for (int i = 0; i < LVLMENU_BTN_COUNT_PER_GROUP; i++) {
		int level = 1;	// index is zero based; levels start with 1
        level += groupid * LVLMENU_BTN_COUNT_PER_GROUP + i;
		m_apxButtons[i]->SetUserData(level);
        
		std::ostringstream oss;
		oss << level;
		m_apxButtons[i]->SetText(oss.str(), 0xffccfaff, Renderer::eFontTypeLarge);
	}

	// make sure they are locked/unlocked
	EnableButtons(true);
}

int LevelMenu::GetNext(int groupid) {
	return (m_iGroupId + 1) % LVLMENU_BTN_GROUP_COUNT;
}

int LevelMenu::GetPrevious(int groupid) {
	return (m_iGroupId + LVLMENU_BTN_GROUP_COUNT - 1) % LVLMENU_BTN_GROUP_COUNT;
}

void LevelMenu::ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args) {
    if (&sender == &m_xButtonPrevious) {
        m_iGroupId = GetPrevious(m_iGroupId);
    } else if (&sender == &m_xButtonNext) {
        m_iGroupId = GetNext(m_iGroupId);
    }
	ApplyGroup(m_iGroupId);
}
