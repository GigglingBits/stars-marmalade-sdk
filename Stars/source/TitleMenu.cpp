#include "TitleMenu.h"
#include "FactoryManager.h"
#include "Debug.h"
#include "SoundEngine.h"
#include "Configuration.h"

TitleMenu::TitleMenu(PageSettings::WorldId world) :
    m_eWorld(PageSettings::eWorldIdEarth),
	m_xButtonCredits(eButtonCommandIdCredits, s3eKeyFirst),
    m_xButtonAchievements(eButtonCommandIdAchievements, s3eKeyFirst),
    m_xButtonPlanet(eButtonCommandIdOpenLevelMenu, s3eKeyEnter),
    m_xButtonPlanetName(eButtonCommandIdOpenLevelMenu, s3eKeyEnter),
    m_xButtonNext(eButtonCommandIdNone, s3eKeyRight),
    m_xButtonPrevious(eButtonCommandIdNone, s3eKeyLeft),
    m_xPanelOptions(eButtonCommandIdOptions, s3eKeyFirst) {

	m_pxBackground = FactoryManager::GetTextureFactory().Create("titlemenu_bg");

    m_xButtonNext.PressedEvent.AddListener<TitleMenu>(this, &TitleMenu::ButtonPressedEventHandler);
    m_xButtonPrevious.PressedEvent.AddListener<TitleMenu>(this, &TitleMenu::ButtonPressedEventHandler);
        
	m_xPanelOptions.StateChanged.AddListener<TitleMenu>(this, &TitleMenu::ButtonPanelStateChangedEventHandler);
        
    m_eWorld = world;
}

TitleMenu::~TitleMenu() {
    m_xButtonNext.PressedEvent.RemoveListener<TitleMenu>(this, &TitleMenu::ButtonPressedEventHandler);
    m_xButtonPrevious.PressedEvent.RemoveListener<TitleMenu>(this, &TitleMenu::ButtonPressedEventHandler);

	m_xPanelOptions.StateChanged.RemoveListener<TitleMenu>(this, &TitleMenu::ButtonPanelStateChangedEventHandler);

	if (m_pxBackground) {
		delete m_pxBackground;
	}

	SoundEngine::StopMusicFile();
}

void TitleMenu::Initialize() {
	m_xButtonCredits.SetTexture(FactoryManager::GetTextureFactory().Create("button_credits"));
	m_xButtonAchievements.SetTexture(FactoryManager::GetTextureFactory().Create("button_achievements"));

    m_xButtonNext.SetTexture(FactoryManager::GetTextureFactory().Create("button_arrow_right"));
	m_xButtonPrevious.SetTexture(FactoryManager::GetTextureFactory().Create("button_arrow_left"));

	m_xPanelOptions.Initialize();
	m_xPanelOptions.GetMainButton().SetTexture(FactoryManager::GetTextureFactory().Create("button_options"));

    m_xButtonPlanet.SetTexture(FactoryManager::GetTextureFactory().Create("button_planet"));
	m_xButtonPlanetName.SetTexture(FactoryManager::GetTextureFactory().Create("button_planet_name"));
    SetWorld(m_eWorld);
    
	SoundEngine::PlayMusicFileLoop(Configuration::GetInstance().IntroSong);
}

void TitleMenu::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xButtonPlanet.Update(frame);
	m_xButtonPlanetName.Update(frame);
	m_xButtonAchievements.Update(frame);
	m_xButtonCredits.Update(frame);
	m_xButtonPrevious.Update(frame);
	m_xButtonNext.Update(frame);
	m_xPanelOptions.Update(frame);

	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
    }
}

void TitleMenu::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.SetViewport(m_xCamera.GetViewport());

    // background
    if (m_pxBackground) {
        VertexStreamScreen shape;        
        const CIwSVec2& screensize = frame.GetScreensize();
        shape.SetRect(CIwRect(0, 0, screensize.x, screensize.y));
        renderer.Draw(shape, *m_pxBackground);
    }
    
    // buttons
	m_xButtonPlanetName.Render(renderer, frame);
	m_xButtonPlanet.Render(renderer, frame);
	m_xButtonAchievements.Render(renderer, frame);
	m_xButtonCredits.Render(renderer, frame);
	m_xButtonPrevious.Render(renderer, frame);
	m_xButtonNext.Render(renderer, frame);
	m_xPanelOptions.Render(renderer, frame);
}

void TitleMenu::OnDoLayout(const CIwSVec2& screensize) {
	int extents = GetScreenExtents();

	// button size
	CIwRect button;
	button.h = extents / 7;						
	button.w = button.h;						

	// location of the buttons
	CIwSVec2 screencenter(screensize.x / 2, screensize.y / 2);
	button.x = screencenter.x - (extents / 2) ;
	button.y = screencenter.y + button.h;
	m_xButtonCredits.SetPosition(button);

	button.x = screencenter.x - (extents / 2) + (1 * button.w);
	button.y = screencenter.y + (extents / 3);
	m_xPanelOptions.GetMainButton().SetPosition(button);
    
	button.x = screencenter.x + (extents / 2) - (2 * button.w);
	m_xButtonAchievements.SetPosition(button);
    
    button.w = (int16)(extents / 1.5f);
    button.h = (int16)(extents / 1.5f);
    button.x = screencenter.x - (button.w / 2);
	button.y = screencenter.y - (button.h / 2);
	m_xButtonPlanet.SetPosition(button);
    
    button.w = extents;
    button.h = extents / 10;
    button.x = screencenter.x - (extents / 2);
	button.y = screencenter.y - (extents / 2) + (extents / 12);
	m_xButtonPlanetName.SetPosition(button);
    
    button.w = extents / 10;
    button.h = extents / 5;
    button.x = screencenter.x - (extents / 2);
	button.y = screencenter.y - (button.h / 2);
	m_xButtonPrevious.SetPosition(button);
    
	button.x = screencenter.x + (extents / 2) - button.w;
	m_xButtonNext.SetPosition(button);
}

void TitleMenu::SetWorld(PageSettings::WorldId world) {
    // communicate the new level
    m_xButtonPlanet.SetUserData((int)world);
    m_xButtonPlanetName.SetUserData((int)world);
    
    // find name for texture frame
    std::string worldframe = PageSettings::GetWorldKey(world);
    
    // background
    if (m_pxBackground) {
        m_pxBackground->SelectFrame(worldframe);
    }
    
    // buttons
    m_xButtonPlanet.SetTextureFrame(worldframe);
    m_xButtonPlanetName.SetTextureFrame(worldframe);
}

void TitleMenu::ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args) {
    if (&sender == &m_xButtonPrevious) {
        m_eWorld = (PageSettings::WorldId)((m_eWorld + PageSettings::eWorldIdMax - 1) % PageSettings::eWorldIdMax);
    } else if (&sender == &m_xButtonNext) {
        m_eWorld = (PageSettings::WorldId)((m_eWorld + 1) % PageSettings::eWorldIdMax);
    }
    SetWorld(m_eWorld);
}

void TitleMenu::ButtonPanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args) {
	ChangeState(args.IsOpen, sender);
}

void TitleMenu::ChangeState(bool enable, const ButtonPanel& except) {
    m_xButtonPlanet.SetEnabled(enable);
    m_xButtonPlanetName.SetEnabled(enable);
	m_xButtonAchievements.SetEnabled(enable);
	m_xButtonCredits.SetEnabled(enable);
	if (&except != &m_xPanelOptions) m_xPanelOptions.SetEnabled(enable);
}
