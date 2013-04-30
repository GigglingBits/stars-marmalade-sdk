#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "Page.h"
#include "Body.h"
#include "AppPanel.h"
#include "GameFoundation.h"
#include "Star.h"
#include "Camera.h"
#include "InputManager.h"
#include "LevelInteractor.h"

#define LEVEL_COMPLETION_DELAY 3000

class Level : public Page {
private:
	CIwFVec2 m_xWorldSize;

	Camera m_xCamera;

	GameFoundation m_xGame;

	AppPanel m_xAppPanel;
	LevelInteractor m_xInteractor;

	int m_iCompletionTimer;

	bool m_bIsPaused;

public:
	Level(const CIwFVec2& worldsize, std::string background);
	virtual ~Level();

	virtual void Initialize();

	void Add(Body* body);

	bool GetCompletionInfo(GameFoundation::CompletionInfo& info);

	GameFoundation& GetGameFoundation();

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);

private:
	void CreateStar();

	void CreateHud();
	void DestroyHud();

	void SetPaused();

	CIwFVec2 CalculateRelativeSoundPosition(const CIwFVec2& worldpos);

	void ButtonPanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args);

	static int32 AppPausedCallback(void* systemData, void* userData);
};

#endif
