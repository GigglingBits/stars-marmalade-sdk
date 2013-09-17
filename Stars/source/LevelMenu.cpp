#include <sstream>

#include "LevelMenu.h"
#include "FactoryManager.h"
#include "Debug.h"
#include "SoundEngine.h"
#include "Configuration.h"

#define NO_BUTTON -1

LevelMenu::LevelMenu(PageSettings::WorldId world) :
    m_xButtonBack(eButtonCommandIdOpenWorldMenu, s3eKeyFirst) {

	m_eWorldId = world;
		
    m_pxBackground = FactoryManager::GetTextureFactory().Create("levelmenu_bg");
    if (m_pxBackground) {
        std::string worldframe = PageSettings::GetWorldKey(world);
        m_pxBackground->SelectFrame(worldframe);
    }

	memset(m_apxButtons, 0, sizeof(m_apxButtons));
}

LevelMenu::~LevelMenu() {
	if (m_pxBackground) {
		delete m_pxBackground;
	}

	for (int i = 0; i < LVLMENU_BTN_COUNT; i++) {
		if (m_apxButtons[i]) {
			delete m_apxButtons[i];
		}
	}

	SoundEngine::StopMusicFile();
}

Button* LevelMenu::GetButton(int col, int row) {
	return m_apxButtons[col + (row * LVLMENU_BTN_COLS)];
}

void LevelMenu::Initialize() {
	for (int i = 0; i < LVLMENU_BTN_COUNT; i++) {
        int level = i + 1; // index is zero based; levels start with 1
        
		if (!m_apxButtons[i]) {
			m_apxButtons[i] = new ButtonEx(eButtonCommandIdOpenLevel, s3eKeyFirst, level);
		}
		
		std::ostringstream oss;
		oss << level;
		m_apxButtons[i]->SetText(oss.str(), 0xffffffff, Renderer::eFontTypeLarge);
		m_apxButtons[i]->SetTexture(FactoryManager::GetTextureFactory().Create("button_level"));
	}

	m_xButtonBack.SetTexture(FactoryManager::GetTextureFactory().Create("button_quit"));

	SoundEngine::PlayMusicFileLoop(Configuration::GetInstance().IntroSong);
}

void LevelMenu::OnUpdate(const FrameData& frame) {
	// update level buttons
	for (int i = 0; i < LVLMENU_BTN_COUNT; i++) {
		if (m_apxButtons[i]) {
			m_apxButtons[i]->Update(frame);
		}
	}

	// update other buttons
	m_xButtonBack.Update(frame);

	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
    }
}

void LevelMenu::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.SetViewport(m_xCamera.GetViewport());

    // background
    if (m_pxBackground) {
        VertexStreamScreen shape;        
        const CIwSVec2& screensize = frame.GetScreensize();
        shape.SetRect(CIwRect(0, 0, screensize.x, screensize.y));
        renderer.Draw(shape, *m_pxBackground);
    }
    
    // level buttons
	for (int i = 0; i < LVLMENU_BTN_COUNT; i++) {
		if (m_apxButtons[i]) {
			m_apxButtons[i]->Render(renderer, frame);
		}
	}

	// other buttons
	m_xButtonBack.Render(renderer, frame);
}

void LevelMenu::OnDoLayout(const CIwSVec2& screensize) {
	int extents = GetScreenExtents();
	int margin = extents / 4;	// 25% 
	int space = margin / 5;		// 25% / 5 = 5%
	CIwSVec2 screencenter(screensize.x / 2, screensize.y / 2);
		
	CIwRect menuarea(0, 0, screensize.x, screensize.y);
	menuarea.x += margin;
	menuarea.y += margin;
	menuarea.w -= 2 * margin;
	menuarea.h -= 2 * margin;

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
	
    // back button
	uint32 btnsize = 60;
	uint32 btnmargin = 15;
	m_xButtonBack.SetPosition(
		CIwRect(btnmargin, btnmargin, btnsize, btnsize));
}

void LevelMenu::ChangeButtonState(bool enable, const ButtonPanel& except) {
	EnableButtons(enable);
}

void LevelMenu::EnableButtons(bool enable) {
	for (int i = 0; i < LVLMENU_BTN_COUNT; i++) {
		if (m_apxButtons[i]) {
			if (enable) {
				m_apxButtons[i]->SetHideWhenDisabled(false);
				m_apxButtons[i]->SetEnabled(CheckLevelConfiguration(m_eWorldId, i + 1));
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
