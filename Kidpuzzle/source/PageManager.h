#ifndef __PAGEMANAGER_H__
#define __PAGEMANAGER_H__

#include "Page.h"
#include "Curtain.h"

class PageManager : public Renderable {
private:
	Page* m_pxNextPage;
	Page* m_pxCurrentPage;

	int m_iInititedLevelId;
	bool m_bRestartRequired;

	Curtain m_xCurtain;

public:
	PageManager();
	~PageManager();

	void NextLevel();
	void RestartLevel();
	void StartLevel(int level);
	void StartLevelMenu();

private:
	Page* CreateDefaultPage();
	Page* CreateNextPage(Page* oldpage);

	std::string GetLevelId(int level);
	void SetNextPage(Page* page);

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
