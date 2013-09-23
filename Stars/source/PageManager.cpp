#include "PageManager.h"
#include "Debug.h"

#include "TitleScreen.h"
#include "WorldMenu.h"
#include "LevelMenu.h"
#include "Preamble.h"
#include "Level.h"
#include "LevelCompletion.h"
#include "FactoryManager.h"

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
}

Page* PageManager::CreateNextPage(Page* oldpage) {
	IW_CALLSTACK_SELF;

	// predefined page flow
	Page* nextpage = NULL;
	if (dynamic_cast<Preamble*>(oldpage)) {
		nextpage = FactoryManager::GetLevelFactory().Create(m_xPageSettings.GetLevelKey());

	} else if (Level* level = dynamic_cast<Level*>(oldpage)) {
		GameFoundation::CompletionInfo info;
		if (level->GetCompletionInfo(info)) {
			nextpage = new LevelCompletion(info);
		} else {
			nextpage = new LevelMenu(m_xPageSettings.GetWorld());
		}

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
	if (FactoryManager::GetPreambleFactory().ConfigExists(m_xPageSettings.GetLevelKey())) {
		SetNextPage(FactoryManager::GetPreambleFactory().Create(m_xPageSettings.GetLevelKey()));
	} else {
		SetNextPage(FactoryManager::GetLevelFactory().Create(m_xPageSettings.GetLevelKey()));
	}
}

void PageManager::StartNextLevel() {
    m_xPageSettings.SetLevel(m_xPageSettings.GetLevel() + 1);
	StartLevel();
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

void PageManager::SetWorld(PageSettings::WorldId world) {
    m_xPageSettings.SetWorld(world);
}

void PageManager::SetLevel(int level) {
    m_xPageSettings.SetLevel(level);
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
			if (m_pxCurrentPage) {
				delete m_pxCurrentPage;
			}
			m_pxCurrentPage = m_pxNextPage;
			if (m_pxCurrentPage) {
				m_pxCurrentPage->Initialize();
			}
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
