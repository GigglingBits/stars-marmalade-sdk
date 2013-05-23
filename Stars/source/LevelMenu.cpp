#include "LevelMenu.h"
#include "FactoryManager.h"
#include "Debug.h"
#include "SoundEngine.h"
#include "Configuration.h"

#define NO_BUTTON -1

LevelMenu::LevelMenu(PageSettings::WorldId world) :
    m_xPanelOptions(eButtonCommandIdOptions, s3eKeyFirst),
    m_xButtonTitleMenu(eButtonCommandIdOpenTitleMenu, s3eKeyFirst),
	m_xButtonAchievements(eButtonCommandIdAchievements, s3eKeyFirst) {

    m_pxBackground = FactoryManager::GetTextureFactory().Create("levelmenu_bg");
    if (m_pxBackground) {
        std::string worldframe = PageSettings::GetWorldKey(world);
        m_pxBackground->SelectFrame(worldframe);
    }

	memset(m_apxButtons, 0, sizeof(m_apxButtons));

	m_xPanelOptions.StateChanged.AddListener<LevelMenu>(this, &LevelMenu::ButtonPanelStateChangedEventHandler);
}

LevelMenu::~LevelMenu() {
	m_xPanelOptions.StateChanged.RemoveListener<LevelMenu>(this, &LevelMenu::ButtonPanelStateChangedEventHandler);

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
			m_apxButtons[i] = new Button(eButtonCommandIdOpenLevel, s3eKeyFirst, level);
		}

		char buf[8];
		snprintf(buf, sizeof(buf), "%i", level);
		m_apxButtons[i]->SetText(buf, 0xffffffff, Renderer::eFontTypeLarge);
		m_apxButtons[i]->SetTexture(FactoryManager::GetTextureFactory().Create("button_level"));
	}

	m_xPanelOptions.Initialize();
	m_xPanelOptions.GetMainButton().SetTexture(FactoryManager::GetTextureFactory().Create("button_options"));

	m_xButtonTitleMenu.SetTexture(FactoryManager::GetTextureFactory().Create("button_quit"));
	m_xButtonAchievements.SetTexture(FactoryManager::GetTextureFactory().Create("button_achievements"));

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
	m_xButtonTitleMenu.Update(frame);
	m_xButtonAchievements.Update(frame);
	m_xPanelOptions.Update(frame);

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
	m_xButtonTitleMenu.Render(renderer, frame);
	m_xButtonAchievements.Render(renderer, frame);
	m_xPanelOptions.Render(renderer, frame);
}

void LevelMenu::OnDoLayout(const CIwSVec2& screensize) {
	int extents = GetScreenExtents();
	int margin = extents / 4;	// 25% 
	int space = margin / 5;		// 25% / 5 = 5%
	CIwSVec2 screencenter(screensize.x / 2, screensize.y / 2);
		
	CIwRect menuarea(0, 0, screensize.x, screensize.y);
	menuarea.x += margin;
	menuarea.y += margin * 4 / 5; // move menu area slightly upwards
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

    // remaining buttons
	button.h = extents / 7;
	button.w = button.h;
    
	button.x = (screensize.x - (3 * button.w) - (2 * space)) / 2;
	button.y = screensize.y * 10 / 13;
	m_xButtonTitleMenu.SetPosition(button);
    
	button.x += button.w + space;
	m_xPanelOptions.GetMainButton().SetPosition(button);
    
	button.x += button.w + space;
	m_xButtonAchievements.SetPosition(button);
}

void LevelMenu::ButtonPanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args) {
	ChangeState(args.IsOpen, sender);
}

void LevelMenu::ChangeState(bool enable, const ButtonPanel& except) {
	for (int i = 0; i < LVLMENU_BTN_COUNT; i++) {
		if (m_apxButtons[i]) {
			m_apxButtons[i]->SetEnabled(enable);
		}
	}

	m_xButtonAchievements.SetEnabled(enable);

	if (&except != &m_xPanelOptions) m_xPanelOptions.SetEnabled(enable);
}
