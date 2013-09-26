#include "WorldMenu.h"
#include "FactoryManager.h"
#include "Debug.h"
#include "SoundEngine.h"
#include "Configuration.h"

WorldMenu::WorldMenu(PageSettings::WorldId world) :
    m_eWorld(world),
    m_xButtonPlanet(eButtonCommandIdOpenLevelMenu, s3eKeyEnter),
    m_xButtonPlanetName(eButtonCommandIdOpenLevelMenu, s3eKeyEnter),
    m_xButtonNext(eButtonCommandIdNone, s3eKeyRight),
    m_xButtonPrevious(eButtonCommandIdNone, s3eKeyLeft),
	m_xButtonBack(eButtonCommandIdOpenTitleScreen, s3eKeyFirst) {

	m_pxBackground = FactoryManager::GetTextureFactory().Create("worldmenu_bg");

    m_xButtonNext.PressedEvent.AddListener<WorldMenu>(this, &WorldMenu::ButtonPressedEventHandler);
    m_xButtonPrevious.PressedEvent.AddListener<WorldMenu>(this, &WorldMenu::ButtonPressedEventHandler);
}

WorldMenu::~WorldMenu() {
    m_xButtonNext.PressedEvent.RemoveListener<WorldMenu>(this, &WorldMenu::ButtonPressedEventHandler);
    m_xButtonPrevious.PressedEvent.RemoveListener<WorldMenu>(this, &WorldMenu::ButtonPressedEventHandler);

	if (m_pxBackground) {
		delete m_pxBackground;
	}

	SoundEngine::StopMusicFile();
}

void WorldMenu::Initialize() {
    m_xButtonNext.SetTexture(FactoryManager::GetTextureFactory().Create("button_arrow_right"));
	m_xButtonPrevious.SetTexture(FactoryManager::GetTextureFactory().Create("button_arrow_left"));

    m_xButtonPlanet.SetTexture(FactoryManager::GetTextureFactory().Create("button_planet"));
	m_xButtonPlanetName.SetTexture(FactoryManager::GetTextureFactory().Create("button_world_name"));
    
	m_xNaviPanel.Initialize();
	m_xNaviPanel.AddButton("navipanel", PageSettings::eWorldIdEarth);
	m_xNaviPanel.AddButton("navipanel", PageSettings::eWorldIdMars);
	m_xNaviPanel.AddButton("navipanel", PageSettings::eWorldIdJupiter);
	
	m_xButtonBack.SetTexture(FactoryManager::GetTextureFactory().Create("button_quit"));

	SoundEngine::PlayMusicFileLoop(Configuration::GetInstance().IntroSong);

    ApplyWorld(m_eWorld);
}

void WorldMenu::OnDoLayout(const CIwSVec2& screensize) {
	CIwSVec2 screencenter(screensize.x / 2, screensize.y / 2);
	CIwRect button;
	int extents = GetScreenExtents();
	
    // name button
	button.w = extents;
    button.h = extents / 10;
    button.x = screencenter.x - (extents / 2);
	button.y = screencenter.y - (extents * 5 / 12);
	m_xButtonPlanetName.SetPosition(button);
    
	// world button
    button.w = (int16)(extents / 1.5f);
    button.h = (int16)(extents / 1.5f);
    button.x = screencenter.x - (button.w / 2);
	button.y = screencenter.y - (button.h / 2);
	m_xButtonPlanet.SetPosition(button);
	
    // navigation buttons
    button.w = extents / 10;
    button.h = extents / 5;
    button.x = screencenter.x - (extents * 3 / 5);
	button.y = screencenter.y - (button.h / 2);
	m_xButtonPrevious.SetPosition(button);
    
	button.x = screencenter.x + (extents * 3 / 5) - button.w;
	m_xButtonNext.SetPosition(button);
	
    // navi panel
	int navidosize = extents / 15;
	button.w = 3 * navidosize;
    button.h = navidosize;
    button.x = screencenter.x - (3 * navidosize / 2);
	button.y = screencenter.y + (extents / 3);
	m_xNaviPanel.SetPosition(button);
	
    // back button
	uint32 btnsize = 60;
	uint32 btnmargin = 15;
	m_xButtonBack.SetPosition(
		CIwRect(btnmargin, btnmargin, btnsize, btnsize));
}

void WorldMenu::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xButtonPlanet.Update(frame);
	m_xButtonPlanetName.Update(frame);

	m_xButtonPrevious.Update(frame);
	m_xButtonNext.Update(frame);

	m_xNaviPanel.Update(frame);
	
	m_xButtonBack.Update(frame);

	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
    }
}

void WorldMenu::OnRender(Renderer& renderer, const FrameData& frame) {
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
	m_xButtonPrevious.Render(renderer, frame);
	m_xButtonNext.Render(renderer, frame);
	m_xNaviPanel.Render(renderer, frame);
	m_xButtonBack.Render(renderer, frame);
}

void WorldMenu::ApplyWorld(PageSettings::WorldId world) {
    // communicate the new level
    m_xButtonPlanet.SetUserData((int)world);
    m_xButtonPlanetName.SetUserData((int)world);
    m_xNaviPanel.ActivateButton((int)world);
	
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

PageSettings::WorldId WorldMenu::GetNext(PageSettings::WorldId worldid) {
	return (PageSettings::WorldId)((m_eWorld + 1) % PageSettings::eWorldIdMax);
}

PageSettings::WorldId WorldMenu::GetPrevious(PageSettings::WorldId worldid) {
	return (PageSettings::WorldId)((m_eWorld + PageSettings::eWorldIdMax - 1) % PageSettings::eWorldIdMax);
}

void WorldMenu::ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args) {
    if (&sender == &m_xButtonPrevious) {
        m_eWorld = GetPrevious(m_eWorld);
    } else if (&sender == &m_xButtonNext) {
        m_eWorld = GetNext(m_eWorld);
    }
    ApplyWorld(m_eWorld);
}
