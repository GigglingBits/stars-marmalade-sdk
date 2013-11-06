#ifndef __LEVELCOMPLETION_H__
#define __LEVELCOMPLETION_H__

#include "Page.h"
#include "Button.h"
#include "Camera.h"
#include "Level.h"
#include "LevelHudNumber.h"

class LevelCompletion : public Page {
private:
	Camera m_xCamera;

	bool m_bIsCompleted;
	std::string m_sCompletionText;

	Button m_xButtonStar;

	Button m_xButtonQuit;
	Button m_xButtonRetry;
	Button m_xButtonNext;
	
	LevelHudNumber m_xDustFillPercent;

	PageBackground m_xBackground;

public:
	LevelCompletion(const Level::CompletionInfo& info);

	virtual void Initialize();

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);

private:
	bool IsCompleted(const Level::CompletionInfo& info);
	std::string GenerateCompletionText(const Level::CompletionInfo& info);
};

#endif
