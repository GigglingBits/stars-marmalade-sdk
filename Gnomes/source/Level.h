#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "Page.h"
#include "Body.h"
#include "AppPanel.h"
#include "AimingDevice.h"
#include "GameFoundation.h"
#include "BackgroundPainter.h"
#include "LevelCompletionInfo.h"
#include "Hero.h"
#include "Camera.h"
#include "InputManager.h"
#include "LevelInteractor.h"
#include "LevelStatsPanel.h"

#define LEVEL_COMPLETION_DELAY 1500

class Level : public Page {
private:
	GameFoundation m_xGame;
	Camera m_xCamera;
	BackgroundPainter m_xBackground;

	AppPanel m_xAppPanel;
	AimingDevice m_xAimingDevice;
	LevelInteractor m_xInteractor;
	LevelStatsPanel m_xStatsPanel;

	std::string m_sHeroInFocus;
	int m_iCompletionTimer;

	bool m_bIsPaused;

public:
	Level(const CIwFVec2& worldsize, std::string background);
	virtual ~Level();

	virtual void Initialize();

	void Add(Body* body);

	bool GetCompletionState(LevelCompletionInfo& info);

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);

private:
	void CreateHud();
	void DestroyHud();

	void SetPaused();

	void SetHeroFocus(const std::string& heroid);
	void ClearHeroFocus();
	bool IsHeroInFocus();

	CIwFVec2 CalculateRelativeSoundPosition(const CIwFVec2& worldpos);

	void SoundEffectEventHandler(const GameFoundation& sender, const GameFoundation::SoundEventArgs& args);
	void HeroChangedEventHandler(const GameFoundation& sender, const GameFoundation::HeroChangedEventArgs& args);
	void HeroFocusChangedEventHandler(const LevelStatsPanel& sender, const std::string& heroid);

	void ButtonPanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args);

	static int32 AppPausedCallback(void* systemData, void* userData);
};

#endif
