#ifndef __LEVELCOMPLETION_H__
#define __LEVELCOMPLETION_H__

#include "Page.h"
#include "PageBackground.h"
#include "Button.h"
#include "Camera.h"
#include "LevelCompletionInfo.h"
#include "LevelHudNumber.h"

class LevelCompletion : public Page {
private:
	Camera m_xCamera;

	std::string m_sLevelId;
	std::string m_sNextLevelId;
	LevelCompletionInfo m_xCompletionInfo;
	
	std::string m_sCompletionText;
	std::string m_sStatsText;

	Button m_xButtonStar;

	Button m_xButtonQuit;
	Button m_xButtonRetry;
	Button m_xButtonNext;
	
	LevelHudNumber m_xDustFillPercent;

	PageBackground m_xBackground;

public:
	LevelCompletion(const std::string levelid, const std::string nextlevelid, const LevelCompletionInfo& info);

	virtual void Initialize();

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);
	
private:
	std::string GetCompletionText();
	std::string GetStatsText();
	void SaveResults();
};

#endif
