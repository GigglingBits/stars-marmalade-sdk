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
		eEventTypeSetTitle,
		eEventTypeSetScore,
		eEventTypeSetBonus,
		eEventTypeTransferBonus,
		eEventTypeClearBonus,
		eEventTypeGiveAwards,
		eEventTypeEnableStar,
	};
	
	struct EventArgs {
		EventType type;
		std::string text;
		int amount;
		EventArgs() {
			type = eEventTypeNoOp;
			amount = 0;
		}
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

	HudText m_xTitleText;
	HudText m_xScoreText;
	HudNumber m_xScoreAmount;
	
	HudText m_xBonusText;
	HudNumber m_xBonusAmount;

	BackgroundParallax m_xBackground;
	VertexStreamScreen m_xBackdropShape;
	
	MulticastEventTimer<EventArgs> m_xEventTimer;
	
	bool m_bEnableStar;

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
	void ScheduleTitle();
	void ScheduleTitleClear();
	void SchedulePoints();
	void SchedulePause(int milliseconds);
	void ScheduleBonus(const std::string& name, int amount);
	void ScheduleAwards(int starcount);
	void ScheduleEnableStar();

	void UpdateStats();
	void SaveStats();

	uint8 Percent(unsigned long base, unsigned long value);
	
private:
	void EventTimerEventHandler(const MulticastEventTimer<EventArgs>& sender, const EventArgs& args);
};

#endif
