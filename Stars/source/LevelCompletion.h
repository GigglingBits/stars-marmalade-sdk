#ifndef __LEVELCOMPLETION_H__
#define __LEVELCOMPLETION_H__

#include "Page.h"
#include "BackgroundParallax.h"
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
	
	Button m_xButtonStar;

	Button m_xButtonQuit;
	Button m_xButtonRetry;
	Button m_xButtonNext;

	BackgroundParallax m_xBackground;

	LevelHudText m_xTitle;
	
	LevelHudText m_xDustAmountText;
	LevelHudNumber m_xDustAmount;
	
	LevelHudText m_xNuggetsCollectedText;
	LevelHudNumber m_xNuggetsCollected;
	
	LevelHudText m_xNumberOfPathsText;
	LevelHudNumber m_xNumberOfPaths;
	
public:
	LevelCompletion(const std::string levelid, const std::string nextlevelid, const LevelCompletionInfo& info);

	virtual void Initialize();

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);
	
private:
	std::string GetCompletionText();
	void SaveResults();
	void SubmitAnalytics();
};

#endif
