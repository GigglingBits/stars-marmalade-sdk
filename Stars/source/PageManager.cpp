#include "PageManager.h"
#include "Debug.h"

#include "TitleScreen.h"
#include "WorldMenu.h"
#include "LevelMenu.h"
#include "Preamble.h"
#include "Level.h"
#include "LevelCompletion.h"
#include "LevelIterator.h"
#include "FactoryManager.h"
#include "ResourceManager.h"
#include "SoundEngine.h"
#include "Configuration.h"
#include "AppAnalytics.h"

PageManager::PageManager() {
	m_pxCurrentPage = NULL;
	m_pxNextPage = NULL;

	m_bRestartRequired = false;
}

PageManager::~PageManager() {
	if (m_pxCurrentPage) {
		delete m_pxCurrentPage;
	}
	if (m_pxNextPage && m_pxNextPage != m_pxCurrentPage) {
		delete m_pxNextPage;
	}
	
	SoundEngine::GetInstance().StopMusicFile();
}

Page* PageManager::CreateNextPage(Page* oldpage) {
	IW_CALLSTACK_SELF;

	// predefined page flow
	Page* nextpage = NULL;
	if (dynamic_cast<Preamble*>(oldpage)) {
		std::string levelname = GetCurrentLevelName();
		nextpage = FactoryManager::GetLevelFactory().Create(levelname);
	} else if (Level* level = dynamic_cast<Level*>(oldpage)) {
		std::string levelname = GetCurrentLevelName();
		std::string nextlevelname = GetNextLevelName();
		const LevelCompletionInfo& info = level->GetCompletionInfo();
		nextpage = new LevelCompletion(levelname, nextlevelname, info);
	} else if (dynamic_cast<LevelCompletion*>(oldpage)) {
		nextpage = new LevelMenu(m_xPageSettings.GetWorld());
	}

	return nextpage;
}

Page* PageManager::CreateDefaultPage() {
	return new TitleScreen();
}

void PageManager::StartLevel() {
	IW_CALLSTACK_SELF;
	std::string levelname = GetCurrentLevelName();
	if (FactoryManager::GetPreambleFactory().ConfigExists(levelname)) {
		SetNextPage(FactoryManager::GetPreambleFactory().Create(levelname));
	} else {
		SetNextPage(FactoryManager::GetLevelFactory().Create(levelname));
	}
}

void PageManager::StartNextLevel() {
	IW_CALLSTACK_SELF;

	// get current state
	LevelIterator::WorldId world = m_xPageSettings.GetWorld();
	int level = m_xPageSettings.GetLevel();
	
	// iterate
	LevelIterator it;
	level = it.GetNextLevelInWorld(world, level);
	
	// manage world transitions, etc
	if (level == LEVELITERATOR_NO_LEVEL) {
		// there seems to be no next level in that world; move to new world
		m_xPageSettings.SetWorld(it.GetNextWorld(world));
		m_xPageSettings.SetLevel(it.GetFirstLevelInWorld(m_xPageSettings.GetWorld()));
		if (m_xPageSettings.GetWorld() == it.GetFirstWorld()) {
			// the previous world was the final world; game completed
			// todo: congratulation screen
			StartTitleScreen();
		} else {
			IwAssertMsg(MYAPP, m_xPageSettings.GetLevel() != LEVELITERATOR_NO_LEVEL, ("No first level found in world '%s'!", it.GetWorldName(world).c_str()));
			StartLevelMenu();
		}
	} else {
		// simply set next level
		m_xPageSettings.SetLevel(level);
		StartLevel();
	}
}

void PageManager::StartTitleScreen() {
	IW_CALLSTACK_SELF;
	SetNextPage(new TitleScreen());
}

void PageManager::StartWorldMenu() {
	IW_CALLSTACK_SELF;
	SetNextPage(new WorldMenu(m_xPageSettings.GetWorld()));
}

void PageManager::StartLevelMenu() {
	IW_CALLSTACK_SELF;
	SetNextPage(new LevelMenu(m_xPageSettings.GetWorld()));
}

void PageManager::SetNextPage(Page* page) {
	if (m_pxCurrentPage != m_pxNextPage) {
		if (m_pxNextPage) {
			delete m_pxNextPage;
		}
	}
	m_pxNextPage = page;
}

void PageManager::SetWorld(LevelIterator::WorldId world) {
    m_xPageSettings.SetWorld(world);
}

void PageManager::SetLevel(int level) {
    m_xPageSettings.SetLevel(level);
}

std::string PageManager::GetCurrentLevelName() {
	LevelIterator it;
	return it.GetLevelName(
						   m_xPageSettings.GetWorld(),
						   m_xPageSettings.GetLevel());
}

std::string PageManager::GetNextLevelName() {
	LevelIterator::WorldId world = m_xPageSettings.GetWorld();
	int level = m_xPageSettings.GetLevel();
	
	LevelIterator it;
	level = it.GetNextLevelInWorld(world, level);
	if (level == LEVELITERATOR_NO_LEVEL) {
		// there seems to be no next level in that world; move to new world
		world = it.GetNextWorld(world);
		level = it.GetFirstLevelInWorld(world);
		IwAssertMsg(MYAPP, level != LEVELITERATOR_NO_LEVEL, ("No first level found in world '%s'!", it.GetWorldName(world).c_str()));
	}
	return it.GetLevelName(world, level);
}

void PageManager::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (!m_pxNextPage) {
		// must always know where to go next
		SetNextPage(CreateDefaultPage());
	}

	if (m_pxCurrentPage != m_pxNextPage) {
		if (m_xCurtain.IsOpen()) {
			// close to start the transition
			m_xCurtain.Close(); 
		}
		if (m_xCurtain.IsClosed()) {
			// do the actual transition
			ApplyNextPage();

			// re-open the curtain
			m_xCurtain.Open();
		}

	} else if (m_pxCurrentPage->IsCompleted()) {
		// check for automatic transitions due to
		// page completion; explicitly request the transition then
		SetNextPage(CreateNextPage(m_pxCurrentPage));
	}

	// regular state update
	if (m_pxCurrentPage) {
		m_pxCurrentPage->Update(frame);
	}

	m_xCurtain.Update(frame);
}

void PageManager::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;
			
	if (m_pxCurrentPage) {
		m_pxCurrentPage->Render(renderer, frame);
	}

	m_xCurtain.Render(renderer, frame);
}

void PageManager::ApplyNextPage() {
	SubmitAnalytics(m_pxCurrentPage, m_pxNextPage);

	if (m_pxCurrentPage) {
		delete m_pxCurrentPage;
	}
	
	m_pxCurrentPage = m_pxNextPage;
	if (m_pxCurrentPage) {
		// load resources associated to that page, if any; otherwise
		// leave the old resources be loaded. They will be re-used or unloaded later.
		std::string groupname = m_pxCurrentPage->GetResourceGroupName();
		if (!groupname.empty()) {
			ResourceManager::GetInstance().LoadTemporary(groupname);
		}
		
		// initialize the page
		m_pxCurrentPage->SetBackground(m_xPageSettings.GetWorld());
		m_pxCurrentPage->Initialize();
		
		// play music associated to the page
		const std::string& musicfile = m_pxCurrentPage->GetMusicFileName();
		if (!musicfile.empty() && SoundEngine::GetInstance().GetPlayingMusicFile() != musicfile) {
			SoundEngine::GetInstance().PlayMusicFileLoop(musicfile);
		}
	}
}

void PageManager::SubmitAnalytics(Page* oldpage, Page* newpage) {
	LevelIterator i;
	std::string worldid(i.GetWorldName(m_xPageSettings.GetWorld()));
	std::string levelid(i.GetLevelName(m_xPageSettings.GetWorld(), m_xPageSettings.GetLevel()));
	
	AppAnalytics a;
	if (dynamic_cast<TitleScreen*>(newpage)) {
		a.RegisterTitleScreenOpened();
	} else if (dynamic_cast<WorldMenu*>(newpage)) {
		a.RegisterWorldMenuOpened();
	} else if (dynamic_cast<LevelMenu*>(newpage)) {
		a.RegisterLevelMenuOpened(worldid);
	} else if (dynamic_cast<Level*>(newpage)) {
		if (dynamic_cast<Level*>(oldpage)) {
			a.RegisterLevelCancelled(levelid);
		}
		a.RegisterLevelStarted(levelid);
	} else if (LevelCompletion* lc = dynamic_cast<LevelCompletion*>(newpage)) {
		const LevelCompletionInfo& info = lc->GetCompletionInfo();
		a.RegisterLevelCompleted(levelid, info.GetDustAmount(), info.GetPathsStarted(), info.IsCleared());
	}
}