#ifndef __LEVELINTERACTOR_H__
#define __LEVELINTERACTOR_H__

#include "GameFoundation.h"
#include "Camera.h"
#include "InputManager.h"
#include "Renderable.h"

#include "TouchRecorder.h"

class LevelInteractor : public Renderable {
private:
	GameFoundation& m_rxGame;
	Camera& m_rxCamera;

	enum eGestureType {
		eGestureTypeNone = 0,
		eGestureTypeDragObject = 1,
		eGestureTypeDrawStarPath = 2
	};

	// monitor the touch input
	struct TouchSpec {
		eGestureType gesturetype;

		CIwFVec2 worldstartpos;
		CIwFVec2 worldendpos;
		CIwSVec2 screenstartpos;
		CIwSVec2 screenendpos;

		std::string targetsprite;
	};
	typedef std::map<TouchId, TouchSpec> TouchMap;
	TouchMap m_xTouchMap;
    
    TouchRecorder m_xRecorder;

public:
	LevelInteractor(Camera& camera, GameFoundation& game);
	~LevelInteractor();

private:
	void EvaluateTouchPurpose(TouchSpec& touch);
	void ClearTouchSpec(TouchSpec& touch);

    virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);

	void TouchBeginEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args);
	void TouchMoveEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args);
	void TouchEndEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args);
	
public:
	struct PathEventArgs {
		int count;
		CIwFVec2* samplepos;
	};
	Event<LevelInteractor, CIwFVec2> BeginDrawPath;
	Event<LevelInteractor, PathEventArgs> EndDrawPath;
};

#endif
