#ifndef __LEVELINTERACTOR_H__
#define __LEVELINTERACTOR_H__

#include "GameFoundation.h"
#include "Camera.h"
#include "InputManager.h"
#include "PinchGesture.h"
/**
* Interaction is processed event drived. 
*
* 1) When a new touch is detected, it is evaluated and recorded. During the evaluation 
*    it is decided what the purpose of the touch is base don what it has hit.
* 2) When a touch is being moved, the motion is recorded. If a touch event is participating 
*    in a pinch gesture, then the pinch is recalculated, and the screen is adjusted.
* 3) When a touch ends, its recording is cleared, and any action it has been involved in will 
*    be finished.
**/
class LevelInteractor {
private:
	GameFoundation& m_rxGame;
	Camera& m_rxCamera;

	enum eGestureType {
		eGestureTypeNone = 0,
		eGestureTypeDragObject = 1,
		eGestureTypeChargeHero = 2,
		eGestureTypePanZoomViewport = 3
	};

	struct PanZoomSpec {
		CIwSVec2 screenpan;
	};

	// monitor the touch input
	struct TouchSpec {
		eGestureType gesturetype;

		CIwFVec2 worldstartpos;
		CIwFVec2 worldendpos;
		CIwSVec2 screenstartpos;
		CIwSVec2 screenendpos;

		std::string targetsprite;
		CIwFVec2 targetspritecenter;
	};
	typedef std::map<TouchId, TouchSpec> TouchRecorder;
	TouchRecorder m_xTouchRecorder;

public:
	struct HeroChargeInfo {
		std::string heroid;
		CIwFVec2 impulse;
		CIwFVec2 heropos;
		HeroChargeInfo() {
			heropos = CIwFVec2::g_Zero;
			impulse = CIwFVec2::g_Zero;
		}
	};

private:
	PinchGesture m_xPinch;
	std::string m_sHeroInFocus;
	HeroChargeInfo m_xHeroCharge;

public:
	LevelInteractor(Camera& camera, GameFoundation& game);
	~LevelInteractor();

	bool IsHeroInFocus();
	const std::string& GetHeroInFocus();

	bool IsHeroCharging();
	const HeroChargeInfo& GetHeroChargeInfo();

	bool IsPinching();
	const PinchGesture::PinchInfo& GetPinchInfo();

private:
	void EvaluateTouchPurpose(TouchSpec& touch);
	void ClearTouchSpec(TouchSpec& touch);

	void SetHeroChargeInfo(const TouchSpec& touch);
	void ClearHeroChargeInfo();

	void TouchBeginEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args);
	void TouchMoveEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args);
	void TouchEndEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args);
};

#endif
