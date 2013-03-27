#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "IwGeom.h"
#include "Viewport.h"

class Camera {
private:
	Viewport m_xViewport;

	// geometry
	CIwFVec2 m_xWorldSize;			// size of world in meters
	CIwFVec2 m_xFocusPoint;			// target position of the viewport

	// zoom
	float m_fW2SFactor;
	float m_fFarViewW2SFactor;		// zoom factor for seeing entire world
	float m_fNearViewW2SFactor;		// zoom factor for near view

	// panning
	struct PanInfo {
		bool IsPanning;
		CIwSVec2 Offset;
		float Scale;
	};
	PanInfo m_xPan;

public:
	Camera();

	// geometry
	void SetGeometry(const CIwFVec2& worldsize, const CIwSVec2& screensize, float maxvisibleworldsize /* used for close view */);

	// moving the camera
	void SetWorldFocus(const CIwFVec2& point);

	void ZoomIn();
	void ZoomOut();

	bool IsPanning();
	void SetScreenPanning(const CIwSVec2& offset, float scale);
	void FinishScreenPanning();

	void Update(const CIwSVec2& screensize, uint16 timestep);

	// expose for coord transformations
	const Viewport& GetViewport();

private:
	float CalcW2SFactorFillScreen(const CIwFVec2& worldsize, const CIwSVec2& screensize, float maxvisibleworldsize);
	float CalcW2SFactorFitInScreen(const CIwFVec2& worldsize, const CIwSVec2& screensize);

	void LimitFocusPoint();
	void LimitZoom();

	float RestrictOverZoom(float scale);
	void RestrictOverPan(CIwSVec2& offset, float scale);

private:
	template <class TPoint, typename TUnit>
	void ConfinePoint(TPoint& point, TUnit l, TUnit r, TUnit b, TUnit t) {
		IwAssert(MYAPP, (l <= r));
		IwAssert(MYAPP, (b <= t));
		point.x = std::min<TUnit>(point.x, r);
		point.x = std::max<TUnit>(point.x, l);
		point.y = std::min<TUnit>(point.y, t);
		point.y = std::max<TUnit>(point.y, b);
	}
};

#endif
