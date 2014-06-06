#ifndef __LEVELINTERACTOR_H__
#define __LEVELINTERACTOR_H__

#include "GameFoundation.h"
#include "Camera.h"
#include "InputManager.h"

#include "PathRecorder.h"
#include "PathTracker.h"

class LevelInteractor {
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
    
	bool m_bInputEnabled;
	PathRecorder m_xRecorder;

public:
	LevelInteractor(Camera& camera, GameFoundation& game);
	~LevelInteractor();

	void Enable();
	void Disable();
	
private:
	void EvaluateTouchPurpose(TouchSpec& touch);
	void ClearTouchSpec(TouchSpec& touch);

	void OnPathChanged(bool complete);
	
	void TouchBeginEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args);
	void TouchMoveEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args);
	void TouchEndEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args);
	
public:
	struct PathEventArgs {
		bool complete;
		std::vector<CIwFVec2> path;
	};
	MulticastEvent<LevelInteractor, PathEventArgs> PathChanged;
};

#endif
