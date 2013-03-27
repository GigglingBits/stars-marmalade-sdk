#include "PinchGesture.h"

PinchGesture::PinchGesture() : m_bTouchesChanged(false) {
	ResetPinchInfo();
}

void PinchGesture::ResetPinchInfo() {
	m_xCurrentPinch.movement = CIwFVec2::g_Zero;
	m_xCurrentPinch.scalefactor = 1.0f;

	m_xCurrentPinch.currentcenter = CIwFVec2::g_Zero;
	m_xCurrentPinch.currentcentercorrection = CIwFVec2::g_Zero;

	m_xCenterCorrection= CIwFVec2::g_Zero;
	m_fScaleFactorCorrection = 1.0f;
}

bool PinchGesture::IsPinching() {
	return !m_xTracker.empty();
}

void PinchGesture::AddTouch(TouchId touchid, const CIwSVec2& pos) {
	PinchInfo oldpinch = Update();

	TouchSpec& touch = m_xTracker[touchid];
	touch.screenstartpos = pos;
	touch.screenendpos = pos;
	m_bTouchesChanged = true;

	PinchInfo& newpinch = Update();
	m_xCenterCorrection += oldpinch.movement - newpinch.movement;
	m_fScaleFactorCorrection *= oldpinch.scalefactor / newpinch.scalefactor;
	m_bTouchesChanged = true;
}

void PinchGesture::MoveTouch(TouchId touchid, const CIwSVec2& pos) {
	TouchSpec& touch = m_xTracker[touchid];
	touch.screenendpos = pos;
	m_bTouchesChanged = true;
}

void PinchGesture::RemoveTouch(TouchId touchid) {
	PinchInfo oldpinch = Update();

	m_xTracker.erase(touchid);
	m_bTouchesChanged = true;

	PinchInfo& newpinch = Update();
	m_xCenterCorrection += oldpinch.movement - newpinch.movement;
	m_fScaleFactorCorrection *= oldpinch.scalefactor / newpinch.scalefactor;
	m_bTouchesChanged = true;
}

const PinchGesture::PinchInfo& PinchGesture::GetPinchInfo() {
	return Update();
}

PinchGesture::PinchInfo& PinchGesture::Update() {
	// previous calculation is still valid
	if (!m_bTouchesChanged) {
		return m_xCurrentPinch;
	}

	m_bTouchesChanged = false;

	// no touches registered
	if (m_xTracker.empty()) {
		ResetPinchInfo();
		return m_xCurrentPinch;
	}

	UpdateMovementAndPinchCenter(m_xTracker, m_xCurrentPinch);
	UpdateScale(m_xTracker, m_xCurrentPinch);

	return m_xCurrentPinch;
}

void PinchGesture::UpdateMovementAndPinchCenter(TouchTracker& tracker, PinchInfo& pinch) {
	// evaluate touches:
	// - movement is calculated as average velocity vectors of all touches
	// - pinch center is calculated as centroid (geometric average of all touches)
	int count = 0;
	CIwFVec2 translationvec = CIwFVec2::g_Zero;
	CIwFVec2 centroidvec = CIwFVec2::g_Zero;
	for (TouchTracker::iterator it = tracker.begin(); it != tracker.end(); it++) {
		// movement
		CIwSVec2& start = it->second.screenstartpos;
		CIwSVec2& end = it->second.screenendpos;
		translationvec += CIwFVec2((float)end.x, (float)end.y) - CIwFVec2((float)start.x, (float)start.y);

		// centroid
		centroidvec.x += (float) end.x;
		centroidvec.y += (float) end.y;

		count++;
	}

	pinch.movement = translationvec / (float)count;
	pinch.currentcenter = centroidvec / (float)count;

	pinch.movement += m_xCenterCorrection;
	pinch.currentcentercorrection = m_xCenterCorrection;
}

void PinchGesture::UpdateScale(TouchTracker& tracker, PinchInfo& pinch) {
	// scaling (possible with at least 2 touches)
	if (m_xTracker.size() > 1) {
		int count = 0;
		float scaleveclen = 0.0f;
		for (TouchTracker::iterator it = tracker.begin(); it != tracker.end(); it++) {
			CIwSVec2& end = it->second.screenendpos;
			scaleveclen += (CIwFVec2((float)end.x, (float)end.y) - m_xCurrentPinch.currentcenter).GetLength();
			count++;
		}
		m_xCurrentPinch.scalefactor = scaleveclen / (float)count;
		m_xCurrentPinch.scalefactor *= m_fScaleFactorCorrection;
	}
}
