#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "Page.h"
#include "Body.h"
#include "AppPanel.h"
#include "GameFoundation.h"
#include "LevelBackground.h"
#include "PageBackground.h"
#include "LevelHud.h"
#include "Star.h"
#include "Camera.h"
#include "LevelInteractor.h"
#include "EventTimer.h"

#define LEVEL_START_BANNER_LEADIN 500
#define LEVEL_START_BANNER_DURATION 1000

#define LEVEL_SETTLE_DELAY 5000

#define LEVEL_SECTION_BANNER_LEADIN 500
#define LEVEL_SECTION_BANNER_DURATION 3000
#define LEVEL_SECTION_BANNER_LEADOUT 500

#define LEVEL_COMPLETION_DELAY 1500
#define LEVEL_LEADOUT_TIME 4000

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
	enum EventId {
		eEventIdNoOp = 0,
		eEventIdShowBanner,
		eEventIdHideBanner,
		eEventIdEnableUserInput,
		eEventIdDisableUserInput,
		eEventIdCreateBody,
		eEventIdSettle,
		eEventIdFinish,
		eEventIdUnload,
	};

	struct EventArgs {
		EventId eventId;
		std::string bannerText;
		std::string bodyName;
		CIwFVec2 position;
		CIwFVec2 speed;
	};
	
private:
	CIwFVec2 m_xWorldSize;

	Camera m_xCamera;

	PageBackground m_xBackgroundStars;
	LevelBackground m_xBackgroundClouds;

	GameFoundation m_xGame;
	CompletionInfo m_xCompletionInfo;

	EventTimer<EventArgs> m_xEventTimer;

	CIwRect m_xBannerRect;
	std::string m_sBannerText;
	
	bool m_bIsPaused;
	bool m_bIsSetteling;
	
	AppPanel m_xAppPanel;
	LevelHud m_xHud;

	LevelInteractor m_xInteractor;

	uint32 m_ulLeadInTime;
	uint32 m_ulLeadOutTime;
	
public:
	Level(const CIwFVec2& worldsize, float dustrequirement);
	virtual ~Level();

	virtual void Initialize();

	void Add(Body* body);
	void Add(uint16 delay, const std::string& body, float ypos, float speed);

	void StartSection(const std::string& icontexture, const std::string& bannertext);
	void EndSection();

	uint32 GetDuration();
	uint32 GetElapsed();
	
	const CompletionInfo& GetCompletionInfo();

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

	void SetPaused(bool paused);
	bool IsPaused();
	static int32 AppPausedCallback(void* systemData, void* userData);

	void ShowBannerText(const std::string& text);
	void HideBannerText();
	void ShowStatsBanner();
	
	void HideStar();
	void ShowStar();
	
	void CreateBody(const std::string& bodyName, const CIwFVec2 pos, const CIwFVec2 speed);
	
	CIwFVec2 CalculateRelativeSoundPosition(const CIwFVec2& worldpos);

	CIwFVec2 GetStarRestPosition();
	CIwFVec2 GetStarHidePosition();
	
	void SetStarAnchor(const CIwFVec2& pos);
	void SetStarPath(int samplecount, const CIwFVec2* samplepoints);

private:
	void EventTimerEventHandler(const EventTimer<EventArgs>& sender, const EventArgs& args);
	void EventTimerClearedEventHandler(const EventTimer<EventArgs>& sender, const int& dummy);
	
	void AppPanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args);

	void BeginDrawPathEventHandler(const LevelInteractor& sender, const CIwFVec2& pos);
	void EndDrawPathHandler(const LevelInteractor& sender, const LevelInteractor::PathEventArgs& path);

	void QuakeImpactEventHandler(const GameFoundation& sender, const GameFoundation::QuakeImpactArgs& args);
};

#endif
