#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "Page.h"
#include "Body.h"
#include "LevelPausedPanel.h"
#include "GameFoundation.h"
#include "BackgroundSprites.h"
#include "BackgroundParallax.h"
#include "LevelHud.h"
#include "Star.h"
#include "PathTracker.h"
#include "Camera.h"
#include "LevelInteractor.h"
#include "MulticastEventTimer.h"
#include "LevelCompletionInfo.h"

#include <vector>

#define LEVEL_START_BANNER_LEADIN 500
#define LEVEL_START_BANNER_DURATION 1000

#define LEVEL_SECTION_BANNER_LEADIN 3000
#define LEVEL_SECTION_BANNER_DURATION 3000
#define LEVEL_SECTION_BANNER_LEADOUT 500

#define LEVEL_COMPLETION_DELAY 1500
#define LEVEL_LEADOUT_TIME 4000

class Level : public Page {
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

	BackgroundParallax m_xBackgroundStars;
	BackgroundSprites m_xBackgroundClouds;

	GameFoundation m_xGame;
	LevelCompletionInfo m_xCompletionInfo;

	MulticastEventTimer<EventArgs> m_xEventTimer;

	CIwRect m_xBannerRect;
	std::string m_sBannerText;
	
	bool m_bIsPaused;
	bool m_bIsSetteling;
	
	LevelPausedPanel m_xPausePanel;
	LevelHud m_xHud;

	LevelInteractor m_xInteractor;
	PathTracker m_xPath;
	
public:
	Level(const CIwFVec2& worldsize, float dustrequirement);
	virtual ~Level();

	virtual void Initialize();

	void Add(Body* body);
	void Add(uint16 delay, const std::string& body, float ypos, float speed);

	void StartSection(const std::string& bannertext);
	void EndSection();

	const LevelCompletionInfo& GetCompletionInfo();

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

	CIwFVec2 GetStarIdlePosition();
	CIwFVec2 GetStarHidePosition();
	
	void SetStarAnchor(const CIwFVec2& pos);
	void SetStarPath(const std::vector<CIwFVec2>& path);

	void Conclude();
	
private:
	void EventTimerEventHandler(const MulticastEventTimer<EventArgs>& sender, const EventArgs& args);
	void EventTimerClearedEventHandler(const MulticastEventTimer<EventArgs>& sender, const int& dummy);
	
	void PausePanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args);

	void PathChangedEventHandler(const LevelInteractor& sender, const LevelInteractor::PathEventArgs& path);

	void QuakeImpactEventHandler(const GameFoundation& sender, const GameFoundation::QuakeImpactArgs& args);
	void SpriteRemovedEventHandler(const GameFoundation& sender, const GameFoundation::SpriteRemovedArgs& args);
};

#endif
