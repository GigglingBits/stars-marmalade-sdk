#include "PageManager.h"
#include "Debug.h"

#include "TitleMenu.h"
#include "LevelMenu.h"
#include "Level.h"
#include "LevelCompletion.h"
#include "FactoryManager.h"

PageManager::PageManager() {
	m_pxCurrentPage = NULL;
	m_pxNextPage = NULL;

	m_bRestartRequired = false;
	m_iInititedLevelId = 0;
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
	if (Level* level = dynamic_cast<Level*>(oldpage)) {
		GameFoundation::CompletionInfo info;
		if (level->GetCompletionInfo(info)) {
			nextpage = new LevelCompletion(info);
		} else {
			nextpage = new LevelMenu();
		}

	} else if (dynamic_cast<LevelCompletion*>(oldpage)) {
		nextpage = new LevelMenu();
	}

	return nextpage;
}

Page* PageManager::CreateDefaultPage() {
	return new TitleMenu();
}

void PageManager::StartLevel(int level) {
	m_iInititedLevelId = level;
	RestartLevel();
}

void PageManager::NextLevel() {
	m_iInititedLevelId++;
	RestartLevel();
}

void PageManager::RestartLevel() {
	IW_CALLSTACK_SELF;
	SetNextPage(FactoryManager::GetLevelFactory().Create(GetLevelId(m_iInititedLevelId)));
}

void PageManager::StartLevelMenu() {
	IW_CALLSTACK_SELF;
	SetNextPage(new LevelMenu());
}

std::string PageManager::GetLevelId(int level) {
	const int len = 8;
	char buf[len];
	snprintf(buf, len, "%i", level);
	return std::string(buf);
}

void PageManager::SetNextPage(Page* page) {
	if (m_pxCurrentPage != m_pxNextPage) {
		if (m_pxNextPage) {
			delete m_pxNextPage;
		}
	}
	m_pxNextPage = page;
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
