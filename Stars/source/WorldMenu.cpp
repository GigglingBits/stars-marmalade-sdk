#include "WorldMenu.h"
#include "FactoryManager.h"
#include "Debug.h"
#include "Configuration.h"
#include "UserSettings.h"

WorldMenu::WorldMenu(LevelIterator::WorldId world) :
	Page("menu.group", Configuration::GetInstance().MenuSong),
    m_eWorld(world),
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
    m_xButtonNext.SetTexture(FactoryManager::GetTextureFactory().Create("button_arrow_right"));
	m_xButtonPrevious.SetTexture(FactoryManager::GetTextureFactory().Create("button_arrow_left"));

    m_xButtonPlanet.SetTexture(FactoryManager::GetTextureFactory().Create("button_planet"));
    m_xButtonPlanet.SetHideWhenDisabled(false);
	
	m_xNaviPanel.Initialize();
	m_xNaviPanel.AddButton("navipanel", LevelIterator::eWorldIdEarth);
	m_xNaviPanel.AddButton("navipanel", LevelIterator::eWorldIdMars);
	m_xNaviPanel.AddButton("navipanel", LevelIterator::eWorldIdJupiter);
	
	m_xButtonBack.SetTexture(FactoryManager::GetTextureFactory().Create("button_quit"));
	m_xBackground.Initialize();

    ApplyWorld(m_eWorld);
}

bool WorldMenu::CheckWorldOpen(LevelIterator::WorldId world) {
	// the first world is always open
	LevelIterator it;
	if (world == it.GetFirstWorld()) {
		return true;
	}

	// unlocked by configuration
	if (Configuration::GetInstance().UnlockAll) {
		return true;
	}
	
	// check if there is an unachieved level in the previous world
	LevelIterator::WorldId previousworld = it.GetPreviousWorld(world);
	int level = it.GetFirstLevelInWorld(previousworld);
	while (level != LEVELITERATOR_NO_LEVEL) {
		std::string levelname = it.GetLevelName(previousworld, level);
		if (UserSettings::GetInstance().GetLevel(levelname).Stars < 1) {
			return false;
		}
		level = it.GetNextLevelInWorld(previousworld, level);
	};

	// this world can be opened
	return true;
}


void WorldMenu::OnDoLayout(const CIwSVec2& screensize) {
	CIwSVec2 screencenter(screensize.x / 2, screensize.y / 2);
	int extents = GetScreenExtents();
	
    // name button
	m_xTitlePos.w = extents;
    m_xTitlePos.h = extents / 10;
    m_xTitlePos.x = screencenter.x - (extents / 2);
	m_xTitlePos.y = screencenter.y - (extents * 5 / 12);
	
	m_xTitleShadowPos = m_xTitlePos;
	m_xTitleShadowPos.x += extents / 150;
	m_xTitleShadowPos.y += extents / 150;
    
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
	
    // navi panel
	int navidosize = extents / 15;
	button.w = 3 * navidosize;
    button.h = navidosize;
    button.x = screencenter.x - (3 * navidosize / 2);
	button.y = screencenter.y + (extents / 3);
	m_xNaviPanel.SetPosition(button);
	
    // back button
	uint32 btnsize = extents / 10;
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
	m_xNaviPanel.Update(frame);
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
	m_xNaviPanel.Render(renderer, frame);
	m_xButtonBack.Render(renderer, frame);
	
	// title
	renderer.DrawText(
		m_sTitle,
		m_xTitleShadowPos,
		Renderer::eFontTypeLarge,
		0xaa444444);
	renderer.DrawText(
		m_sTitle,
		m_xTitlePos,
		Renderer::eFontTypeLarge,
		0xffccfaff);
}

void WorldMenu::ApplyWorld(LevelIterator::WorldId world) {
    // communicate the new level
    m_xButtonPlanet.SetUserData((int)world);
    m_xNaviPanel.ActivateButton((int)world);

    // buttons
	LevelIterator iterator;
	bool open = CheckWorldOpen(world);
	m_xButtonPlanet.SetEnabled(open);

	std::string worldframe = iterator.GetWorldName(world);
	if (!open) {
		worldframe += "_locked";
	}
    m_xButtonPlanet.SetTextureFrame(worldframe);

	// title text
	switch (world) {
		case LevelIterator::eWorldIdEarth:
		{
			m_sTitle = "The blue planet";
			break;
		}
		case LevelIterator::eWorldIdMars:
		{
			m_sTitle = "The planet of fire";
			break;
		}
		case LevelIterator::eWorldIdJupiter:
		{
			m_sTitle = "The planet of rocks";
			break;
		}
		default:
			m_sTitle = "Some Planet";
	}
	
	// update background
	PageSettings ps;
	ps.SetWorld(world);
	SetBackground(
		ps.GetWorldColours().LowerLeft,
		ps.GetWorldColours().LowerRight,
		ps.GetWorldColours().UpperRight,
		ps.GetWorldColours().UpperLeft);
}

LevelIterator::WorldId WorldMenu::GetNext(LevelIterator::WorldId world) {
	LevelIterator it;
	return it.GetNextWorld(world);
}

LevelIterator::WorldId WorldMenu::GetPrevious(LevelIterator::WorldId world) {
	LevelIterator it;
	return it.GetPreviousWorld(world);
}

void WorldMenu::ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args) {
    if (&sender == &m_xButtonPrevious) {
        m_eWorld = GetPrevious(m_eWorld);
    } else if (&sender == &m_xButtonNext) {
        m_eWorld = GetNext(m_eWorld);
    }
    ApplyWorld(m_eWorld);
}
