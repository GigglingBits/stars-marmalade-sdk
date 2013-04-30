#ifndef __LEVELCOMPLETION_H__
#define __LEVELCOMPLETION_H__

#include "Page.h"
#include "Button.h"
#include "Camera.h"
#include "GameFoundation.h"

class LevelCompletion : public Page {
private:
	Camera m_xCamera;

	Texture* m_pxBackground;

	bool m_bIsCompleted;
	std::string m_sCompletionText;

	Button m_xButtonStar;

	Button m_xButtonQuit;
	Button m_xButtonRetry;
	Button m_xButtonNext;

public:
	LevelCompletion(GameFoundation::CompletionInfo& info);
	~LevelCompletion();

	virtual void Initialize();

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);

private:
	bool IsCompleted(GameFoundation::CompletionInfo& info);
	std::string GenerateCompletionText(GameFoundation::CompletionInfo& info);
};

#endif
