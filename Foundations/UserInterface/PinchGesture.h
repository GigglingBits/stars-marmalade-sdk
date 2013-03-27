#ifndef __PINCHGESTURE_H__
#define __PINCHGESTURE_H__

#include "InputManager.h"

class PinchGesture {
public:
	struct PinchInfo {
		CIwFVec2 movement;
		float scalefactor;

		CIwFVec2 currentcenter;				// center of the pinch
		CIwFVec2 currentcentercorrection;	// offset can occur if fingers are added/removed from the pinch
	};

private:
	struct TouchSpec {
		CIwSVec2 screenstartpos;
		CIwSVec2 screenendpos;
	};
	typedef std::map<TouchId, TouchSpec> TouchTracker;
	TouchTracker m_xTracker;

	PinchInfo m_xCurrentPinch;
	bool m_bTouchesChanged;

	// used to support continuous pinching even when
	// touches are added or removed
	CIwFVec2 m_xCenterCorrection;
	float m_fScaleFactorCorrection;

public:
	PinchGesture();

	bool IsPinching();

	void AddTouch(TouchId touchid, const CIwSVec2& pos);
	void MoveTouch(TouchId touchid, const CIwSVec2& pos);
	void RemoveTouch(TouchId touchid);

	const PinchInfo& GetPinchInfo();

private:
	void ResetPinchInfo();
	PinchInfo& Update();
	void UpdateMovementAndPinchCenter(TouchTracker& tracker, PinchInfo& pinch);
	void UpdateScale(TouchTracker& tracker, PinchInfo& pinch);
};

#endif
