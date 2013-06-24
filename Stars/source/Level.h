#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "Page.h"
#include "Body.h"
#include "AppPanel.h"
#include "GameFoundation.h"
#include "LevelBackground.h"
#include "Star.h"
#include "Camera.h"
#include "LevelInteractor.h"
#include "LevelStatsPanel.h"
#include "ButtonEx.h"

#define LEVEL_COMPLETION_DELAY 3000

class Level : public Page {
private:
	CIwFVec2 m_xWorldSize;

	Camera m_xCamera;
	LevelBackground m_xBackground;

	GameFoundation m_xGame;

	AppPanel m_xAppPanel;

	ButtonEx m_xButtonBlock;
	ButtonEx m_xButtonHit;
	ButtonEx m_xButtonAttack;
	
	LevelInteractor m_xInteractor;
	LevelStatsPanel m_xStatsPanel;

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
	void ButtonPanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args);

	void ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args);
	void ButtonReleasedEventHandler(const Button& sender, const Button::EventArgs& args);

	float GetStarMoveForce();
	float GetStarRestForce();
	CIwFVec2 GetStarStartPosition();

	void BeginDrawPathEventHandler(const LevelInteractor& sender, const CIwFVec2& pos);
	void EndDrawPathHandler(const LevelInteractor& sender, const LevelInteractor::PathEventArgs& path);
	
	void SetPaused();
	static int32 AppPausedCallback(void* systemData, void* userData);

	CIwFVec2 CalculateRelativeSoundPosition(const CIwFVec2& worldpos);
};

#endif
