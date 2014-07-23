#ifndef __LEVELCOMPLETION_H__
#define __LEVELCOMPLETION_H__

#include "Page.h"
#include "BackgroundParallax.h"
#include "Button.h"
#include "Camera.h"
#include "LevelCompletionInfo.h"
#include "HudNumber.h"
#include "MulticastEventTimer.h"

class LevelCompletion : public Page {
private:
	enum EventType {
		eEventTypeNoOp = 0,
		eEventTypeSetScore,
		eEventTypeSetBonus,
		eEventTypeTransferBonus,
		eEventTypeClearBonus,
		eEventTypeSetStars,
	};
	
	struct EventArgs {
		EventType type;
		std::string text;
		int amount;
	};

private:
	Camera m_xCamera;

	std::string m_sLevelId;
	std::string m_sNextLevelId;
	LevelCompletionInfo m_xCompletionInfo;
	
	Texture* m_pxStar;
	VertexStreamScreen m_xStarShape;
	
	Texture* m_pxAward;
	VertexStreamScreen m_xAwardShape;
	
	Button m_xButtonQuit;
	Button m_xButtonRetry;
	Button m_xButtonNext;

	HudText m_xScoreText;
	HudNumber m_xScoreAmount;
	
	HudText m_xBonusText;
	HudNumber m_xBonusAmount;

	BackgroundParallax m_xBackground;
	VertexStreamScreen m_xBackdropShape;
	
	MulticastEventTimer<EventArgs> m_xEventTimer;

public:
	LevelCompletion(const std::string levelid, const std::string nextlevelid, const LevelCompletionInfo& info);
	~LevelCompletion();
	
	virtual void Initialize();

	const LevelCompletionInfo& GetCompletionInfo();
	
protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);
	
private:	
	void ScheduleEvents();
	void SchedulePoints();
	void ScheduleBonus(const std::string& name, int amount);
	void ScheduleStars(int count);

	void SaveResults();
	
private:
	void EventTimerEventHandler(const MulticastEventTimer<EventArgs>& sender, const EventArgs& args);
};

#endif
