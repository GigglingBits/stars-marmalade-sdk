#include "WorldMenu.h"
#include "FactoryManager.h"
#include "Debug.h"
#include "Configuration.h"
#include "UserSettings.h"

WorldMenu::WorldMenu() :
	Page("menu.group", Configuration::GetInstance().MenuSong),
    m_xButtonPlanet(eButtonCommandIdOpenLevelMenu, s3eKeyAbsOk),
    m_xButtonNext(eButtonCommandIdNone, s3eKeyAbsRight),
    m_xButtonPrevious(eButtonCommandIdNone, s3eKeyAbsLeft),
	m_xButtonBack(eButtonCommandIdOpenTitleScreen, s3eKeyAbsGameD) {

    m_xButtonNext.PressedEvent.AddListener<WorldMenu>(this, &WorldMenu::ButtonPressedEventHandler);
    m_xButtonPrevious.PressedEvent.AddListener<WorldMenu>(this, &WorldMenu::ButtonPressedEventHandler);
}

WorldMenu::~WorldMenu() {
    m_xButtonNext.PressedEvent.RemoveListener<WorldMenu>(this, &WorldMenu::ButtonPressedEventHandler);
    m_xButtonPrevious.PressedEvent.RemoveListener<WorldMenu>(this, &WorldMenu::ButtonPressedEventHandler);
}

void WorldMenu::Initialize() {
	m_xButtonPlanet.SetTexture(FactoryManager::GetTextureFactory().Create("button_planet"));
    m_xButtonNext.SetTexture(FactoryManager::GetTextureFactory().Create("button_arrow_right"));
	m_xButtonPrevious.SetTexture(FactoryManager::GetTextureFactory().Create("button_arrow_left"));
	
	m_xButtonBack.SetTexture(FactoryManager::GetTextureFactory().Create("button_quit"));
	m_xBackground.Initialize();
}

void WorldMenu::OnDoLayout(const CIwSVec2& screensize) {
	CIwSVec2 screencenter(screensize.x / 2, screensize.y / 2);
	int extents = GetScreenExtents();
	
    // name button
	m_xTitlePos.w = extents;
    m_xTitlePos.h = extents / 10;
    m_xTitlePos.x = screencenter.x - (extents / 2);
	m_xTitlePos.y = screencenter.y - (extents * 5 / 12);
	
	// world button
	CIwRect button;
    button.w = (int16)(extents / 1.5f);
    button.h = (int16)(extents / 1.5f);
    button.x = screencenter.x - (button.w / 2);
	button.y = screencenter.y - (button.h / 2);
	m_xButtonPlanet.SetPosition(button);
	
    // navigation buttons
    button.w = extents / 5;
    button.h = extents / 5;
    button.x = screencenter.x - (extents * 5 / 8);
	button.y = screencenter.y - (button.h / 2);
	m_xButtonPrevious.SetPosition(button);
    
	button.x = screencenter.x + (extents * 5 / 8) - button.w;
	m_xButtonNext.SetPosition(button);
	
    // back button
	uint32 btnsize = extents / 8;
	uint32 btnmargin = extents / 30;
	m_xButtonBack.SetPosition(
		CIwRect(screensize.x - (btnsize + btnmargin),
				btnmargin, btnsize, btnsize));
}

void WorldMenu::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xButtonPlanet.Update(frame);
	m_xButtonPrevious.Update(frame);
	m_xButtonNext.Update(frame);
	m_xButtonBack.Update(frame);

	m_xBackground.Update(frame);
}

void WorldMenu::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.SetViewport(m_xCamera.GetViewport());
	m_xBackground.Render(renderer, frame);
	   
    // buttons
	m_xButtonPlanet.Render(renderer, frame);
	m_xButtonPrevious.Render(renderer, frame);
	m_xButtonNext.Render(renderer, frame);
	m_xButtonBack.Render(renderer, frame);
	
	// title
	renderer.DrawText(
		m_sTitle,
		m_xTitlePos,
		Renderer::eFontTypeLarge,
		GAME_COLOUR_FONT_MAIN);
}

void WorldMenu::ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args) {
	;;
}
