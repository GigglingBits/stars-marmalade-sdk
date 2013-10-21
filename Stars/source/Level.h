#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "Page.h"
#include "Body.h"
#include "AppPanel.h"
#include "GameFoundation.h"
#include "LevelBackground.h"
#include "LevelHud.h"
#include "Star.h"
#include "Camera.h"
#include "LevelInteractor.h"
#include "EventTimer.h"

#define LEVEL_COMPLETION_DELAY 10000

class Level : public Page {
public:
	struct CompletionInfo {
		bool IsCleared;
		float DustFillPercent;
		CompletionInfo() {
			IsCleared = false;
			DustFillPercent = 0.0f;
		}
	};
	
private:
	CIwFVec2 m_xWorldSize;

	Camera m_xCamera;
	LevelBackground m_xBackground;

	GameFoundation m_xGame;
	CompletionInfo m_xCompletionInfo;

	struct BodySpec {
		std::string Body;
		float YPos;
	};
	EventTimer<BodySpec> m_xBodyTimer;

	bool m_bIsPaused;
	
	AppPanel m_xAppPanel;
	LevelHud m_xHud;

	LevelInteractor m_xInteractor;

	int m_iCompletionTimer;

public:
	Level(const CIwFVec2& worldsize, float dustrequirement, std::string background);
	virtual ~Level();

	virtual void Initialize();

	void Add(Body* body);
	void Add(uint16 delay, const std::string& body, float ypos);

	const CompletionInfo& GetCompletionInfo();
	float GetCompletionDegree();

	GameFoundation& GetGameFoundation();

	virtual const std::string& GetResourceGroupName();
	
protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);

private:
	void CreateStar();

	void CreateHud();
	void DestroyHud();

	float GetStarMoveForce();
	float GetStarRestForce();
	CIwFVec2 GetStarStartPosition();

	void SetPaused(bool paused);
	bool IsPaused();
	static int32 AppPausedCallback(void* systemData, void* userData);

	CIwFVec2 CalculateRelativeSoundPosition(const CIwFVec2& worldpos);
	
private:
	void BodyTimerEventHandler(const EventTimer<BodySpec>& sender, const BodySpec& args);
	void BodyTimerClearedEventHandler(const EventTimer<BodySpec>& sender, const int& dummy);
	
	void AppPanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args);

	void BeginDrawPathEventHandler(const LevelInteractor& sender, const CIwFVec2& pos);
	void EndDrawPathHandler(const LevelInteractor& sender, const LevelInteractor::PathEventArgs& path);

	void QuakeImpactEventHandler(const GameFoundation& sender, const GameFoundation::QuakeImpactArgs& args);
};

#endif
