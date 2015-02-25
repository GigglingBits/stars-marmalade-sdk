#include "PageManager.h"
#include "Debug.h"

#include "TitleScreen.h"
#include "IntroMovie.h"
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
		nextpage = new LevelMenu();
	} else if (dynamic_cast<IntroMovie*>(oldpage)) {
		nextpage = new TitleScreen();
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
	int level = m_xPageSettings.GetLevel();
	
	// iterate
	LevelIterator it;
	level = it.GetNextLevel(level);
	
	// manage world transitions, etc
	if (level == LEVELITERATOR_NO_LEVEL) {
		// there seems to be no next level in that world; move to new world
		m_xPageSettings.SetLevel(it.GetFirstLevel());
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

void PageManager::StartIntroMovie() {
	IW_CALLSTACK_SELF;
	SetNextPage(new IntroMovie(Configuration::GetInstance().IntroMovie));
}

void PageManager::StartWorldMenu() {
	IW_CALLSTACK_SELF;
	SetNextPage(new WorldMenu());
}

void PageManager::StartLevelMenu() {
	IW_CALLSTACK_SELF;
	SetNextPage(new LevelMenu());
}

void PageManager::SetNextPage(Page* page) {
	if (m_pxCurrentPage != m_pxNextPage) {
		if (m_pxNextPage) {
			delete m_pxNextPage;
		}
	}
	m_pxNextPage = page;
}

void PageManager::SetLevel(int level) {
    m_xPageSettings.SetLevel(level);
}

std::string PageManager::GetCurrentLevelName() {
	LevelIterator it;
	return it.GetLevelName(m_xPageSettings.GetLevel());
}

std::string PageManager::GetNextLevelName() {
	int level = m_xPageSettings.GetLevel();
	
	LevelIterator it;
	level = it.GetNextLevel(level);
	if (level == LEVELITERATOR_NO_LEVEL) {
		// there seems to be no next level in that world; move to new world
		level = it.GetFirstLevel();
		IwAssertMsg(MYAPP, level != LEVELITERATOR_NO_LEVEL, ("No first level found!"));
	}
	return it.GetLevelName(level);
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
		m_pxCurrentPage->SetBackground(FactoryManager::GetTextureFactory().Create("background"));
		m_pxCurrentPage->Initialize();
		
		// play music associated to the page
		const std::string& musicfile = m_pxCurrentPage->GetMusicFileName();
		if (SoundEngine::GetInstance().GetPlayingMusicFile() != musicfile) {
			if (musicfile.empty()) {
				SoundEngine::GetInstance().StopMusicFile();
			} else {
				SoundEngine::GetInstance().PlayMusicFileLoop(musicfile);
			}
		}
	}
}

void PageManager::SubmitAnalytics(Page* oldpage, Page* newpage) {
	LevelIterator i;
	std::string levelid(i.GetLevelName(m_xPageSettings.GetLevel()));
	
	AppAnalytics a;
	if (dynamic_cast<TitleScreen*>(newpage)) {
		a.RegisterTitleScreenOpened();
	} else if (dynamic_cast<WorldMenu*>(newpage)) {
		a.RegisterWorldMenuOpened();
	} else if (dynamic_cast<LevelMenu*>(newpage)) {
		a.RegisterLevelMenuOpened();
	} else if (dynamic_cast<Level*>(newpage)) {
		if (dynamic_cast<Level*>(oldpage)) {
			a.RegisterLevelCancelled(levelid);
		}
		a.RegisterLevelStarted(levelid);
	} else if (LevelCompletion* lc = dynamic_cast<LevelCompletion*>(newpage)) {
		const LevelCompletionInfo& info = lc->GetCompletionInfo();
		a.RegisterLevelCompleted(levelid, info.GetTotalPoints(), info.GetAchievedStars(), info.IsLevelAchieved());
	}
}