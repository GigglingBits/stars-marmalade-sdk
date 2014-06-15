#ifndef __LEVELCOMPLETION_H__
#define __LEVELCOMPLETION_H__

#include "Page.h"
#include "BackgroundParallax.h"
#include "Button.h"
#include "Camera.h"
#include "LevelCompletionInfo.h"
#include "HudNumber.h"

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

	HudText m_xTitle;
	
	HudText m_xDustAmountText;
	HudNumber m_xDustAmount;
	
	HudText m_xNuggetsCollectedText;
	HudNumber m_xNuggetsCollected;
	
	HudText m_xNumberOfPathsText;
	HudNumber m_xNumberOfPaths;
	
public:
	LevelCompletion(const std::string levelid, const std::string nextlevelid, const LevelCompletionInfo& info);

	virtual void Initialize();

	const LevelCompletionInfo& GetCompletionInfo();
	
protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);
	
private:
	std::string GetCompletionText();
	void SaveResults();
};

#endif
