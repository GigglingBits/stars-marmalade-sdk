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
    SetWorld(m_eWorld);
    
	m_xButtonBack.SetTexture(FactoryManager::GetTextureFactory().Create("button_quit"));

	SoundEngine::PlayMusicFileLoop(Configuration::GetInstance().IntroSong);
}

void WorldMenu::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xButtonPlanet.Update(frame);
	m_xButtonPlanetName.Update(frame);
	m_xButtonPrevious.Update(frame);
	m_xButtonNext.Update(frame);

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
	m_xButtonBack.Render(renderer, frame);
}

void WorldMenu::OnDoLayout(const CIwSVec2& screensize) {
	int extents = GetScreenExtents();

	// button size
	CIwRect button;
	button.h = extents / 7;						
	button.w = button.h;						

	// location of the buttons
	CIwSVec2 screencenter(screensize.x / 2, screensize.y / 2);
    button.w = (int16)(extents / 1.3f);
    button.h = (int16)(extents / 1.3f);
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
	
    // back button
	uint32 btnsize = 60;
	uint32 btnmargin = 15;
	m_xButtonBack.SetPosition(
		CIwRect(screensize.x - (btnsize + btnmargin),
		btnmargin, btnsize, btnsize));
}

void WorldMenu::SetWorld(PageSettings::WorldId world) {
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

void WorldMenu::ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args) {
    if (&sender == &m_xButtonPrevious) {
        m_eWorld = (PageSettings::WorldId)((m_eWorld + PageSettings::eWorldIdMax - 1) % PageSettings::eWorldIdMax);
    } else if (&sender == &m_xButtonNext) {
        m_eWorld = (PageSettings::WorldId)((m_eWorld + 1) % PageSettings::eWorldIdMax);
    }
    SetWorld(m_eWorld);
}

void WorldMenu::ChangeButtonState(bool enable, const ButtonPanel& except) {
    m_xButtonPlanet.SetEnabled(enable);
    m_xButtonPlanetName.SetEnabled(enable);
	m_xButtonPrevious.SetEnabled(enable);
	m_xButtonNext.SetEnabled(enable);
}
