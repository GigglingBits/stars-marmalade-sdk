#ifndef __LEVELCOMPLETION_H__
#define __LEVELCOMPLETION_H__

#include "Page.h"
#include "Button.h"
#include "LevelCompletionInfo.h"
#include "Camera.h"

class LevelCompletion : public Page {
private:
	Camera m_xCamera;

	Texture* m_pxBackground;

	bool m_bIsCompleted;
	std::string m_sCompletionText;

	Button m_xButtonQuit;
	Button m_xButtonRetry;
	Button m_xButtonNext;

public:
	LevelCompletion(LevelCompletionInfo& info);
	~LevelCompletion();

	virtual void Initialize();

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);

private:
	bool IsCompleted(LevelCompletionInfo& info);
	std::string GenerateCompletionText(LevelCompletionInfo& info);
};

#endif
