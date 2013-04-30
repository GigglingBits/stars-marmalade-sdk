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
	Button m_xButtonQuit;

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
};

#endif
