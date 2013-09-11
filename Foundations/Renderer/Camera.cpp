#include <cmath>
#include <algorithm>

#include "Camera.h"

Camera::Camera() {

	m_xWorldSize = CIwFVec2::g_Zero;
	m_xFocusPoint = CIwFVec2::g_Zero;

	m_fW2SFactor = 1.0f;

	m_fNearViewW2SFactor = 50.0f;
	m_fFarViewW2SFactor = 50.0f;

	m_xPan.IsPanning = false;
	m_xPan.Offset = CIwSVec2::g_Zero;
	m_xPan.Scale = 1.0f;
}

void Camera::SetGeometry(const CIwFVec2& worldsize, const CIwSVec2& screensize, float maxvisibleworldsize) {
	// store geometry
	m_xWorldSize = worldsize;
	m_xViewport.SetGeometry(worldsize, screensize);

	// define the zoom
	m_fNearViewW2SFactor = CalcW2SFactorFillScreen(worldsize, screensize, maxvisibleworldsize);
	m_fFarViewW2SFactor = CalcW2SFactorFitInScreen(worldsize, screensize);
	m_xViewport.SetWorldToScreenFactor(m_fNearViewW2SFactor);
	m_fW2SFactor = m_fNearViewW2SFactor;
}

void Camera::LimitZoom() {
	m_fW2SFactor = std::min<float>(m_fW2SFactor, m_fNearViewW2SFactor);
	m_fW2SFactor = std::max<float>(m_fW2SFactor, m_fFarViewW2SFactor);
}

void Camera::ZoomIn() {
	m_fW2SFactor = m_fNearViewW2SFactor;
}

void Camera::ZoomOut() {
	m_fW2SFactor = m_fFarViewW2SFactor;
}

void Camera::SetWorldFocus(const CIwFVec2& point) {
	IwAssertMsg(MYAPP, !IsPanning(), ("World focus must not be set while the viewport is panning."));

	// find desired viewport position (point is centered)
	m_xFocusPoint = point;

	// make sure that the point is not outside the designed world
	LimitFocusPoint();
}

void Camera::LimitFocusPoint() {
	// The focus point in normal camera operation (i.e not when panning)
	// will always be located inside the designed world. The camera will 
	// not follow objects outside this border.

	float w2s = m_xViewport.GetWorldToScreenFactor();
	const CIwSVec2& viewportsizepx = m_xViewport.GetViewportSize();
	CIwFVec2 margin(viewportsizepx.x / w2s, viewportsizepx.y / w2s);
	margin *= 0.5f; // half because the point is centered in the view

	float l = margin.x;
	float r = m_xWorldSize.x - margin.x;
	float b = margin.y;
	float t = m_xWorldSize.y - margin.y;
	
	if (l > r) {
		l = (l + r) / 2.0f;
		r = l;
	}
	if (b > t) {
		b = (b + t) / 2.0f;
		t = b;
	}

	ConfinePoint<CIwFVec2, float>(m_xFocusPoint, l, r, b, t);
}

void Camera::SetScreenPanning(const CIwSVec2& offset, float scale) {
	m_xPan.IsPanning = true;

	// make sure that we don't pan overpan (only allow overpanning with one viewport size) 
	// Overpanning occurs when panning causes the camera to show
	// the space outside the designed world. Overpanning is allowed, 
	// but limited to a margin equal to one viewsize outside the world. 
	m_xPan.Scale = RestrictOverZoom(scale);
	m_xPan.Offset = offset;
	RestrictOverPan(m_xPan.Offset, m_xPan.Scale);
}

void Camera::FinishScreenPanning() {
	// panning has finished;
	m_xPan.IsPanning = false;

	// set new position
	CIwSVec2 screenpos = m_xViewport.WorldToScreen(m_xFocusPoint) + m_xPan.Offset;
	m_xPan.Offset = CIwSVec2::g_Zero;
	SetWorldFocus(m_xViewport.ScreenToWorld(screenpos));

	// new zoom
	m_fW2SFactor *= m_xPan.Scale;
	m_xPan.Scale = 1.0f;
	LimitZoom();
}

bool Camera::IsPanning()  {
	return m_xPan.IsPanning;
}

float Camera::RestrictOverZoom(float scale) {
	const float damping = 0.3f;		// the smaller the value, to harder the damping
	const float maxerror = 0.2f;	// the smaller the value, the smaller allowed the overzoom

	float farscale = m_fFarViewW2SFactor / m_fW2SFactor;
	if (scale < farscale) {			
		float toosmall = (((farscale / scale) - 1.0f) * damping) + 1.0f;
		scale = farscale / std::min<float>(toosmall, 1.0f + maxerror);
	} else {
		float nearscale = m_fNearViewW2SFactor / m_fW2SFactor;
		if (scale > nearscale) {
			float toolarge = (((scale / nearscale) - 1.0f) * damping) + 1.0f;
			scale = nearscale * std::min<float>(toolarge, 1.0f + maxerror);
		}
	}
	return scale;
}

void Camera::RestrictOverPan(CIwSVec2& offset, float scale) {
	float panw2s = m_xViewport.GetWorldToScreenFactor() * scale;
	CIwSVec2 worldsize((int16)(m_xWorldSize.x * panw2s), (int16)(m_xWorldSize.y * panw2s));

	const CIwSVec2& viewportsize = m_xViewport.GetViewportSize();
	CIwSVec2 margin(viewportsize.x / 2, viewportsize.y / 2); // half because the point is centered in the view
	CIwSVec2 focuspoint = m_xViewport.WorldToScreen(m_xFocusPoint);

	int16 l = -margin.x - focuspoint.x;
	int16 r = worldsize.x + margin.x - focuspoint.x;
	int16 b = -margin.y - focuspoint.y;
	int16 t = worldsize.y + margin.y - focuspoint.y;

	ConfinePoint<CIwSVec2, int16>(offset, l, r, b, t);
}

void Camera::Update(const CIwSVec2& screensize, uint16 timestep) {
	// todo: make independent of fps 
	const float threshold = 0.01f;
	const float factor = 0.2f;

	// zoom
	if (m_xPan.IsPanning) {
		// stick to the finger
		m_xViewport.SetWorldToScreenFactor(m_fW2SFactor * m_xPan.Scale);
		if (m_xPan.Scale < 1.0f) {
			// zooming out can cause viewport to show 
			// non-designed area of the world; repositioning may be needed
			LimitFocusPoint();
		}
	} else {
		// automatic zoom
		float currentfactor = m_xViewport.GetWorldToScreenFactor();
		float difffactor = m_fW2SFactor - currentfactor;
		if (std::fabs(difffactor) > threshold) {
			m_xViewport.SetWorldToScreenFactor(currentfactor + (factor * difffactor));
			if (difffactor < 0.0f) {
				// zooming out can cause viewport to show 
				// non-designed area of the world; repositioning may be needed
				LimitFocusPoint();
			}
		}
	}

	// position
	if (m_xPan.IsPanning) {
		// stick to the finger
		CIwSVec2 screenpos = m_xViewport.WorldToScreen(m_xFocusPoint) + m_xPan.Offset;
		m_xViewport.SetCenterPosition(m_xViewport.ScreenToWorld(screenpos));
	} else {
		// attempt automatic screen positioning
		CIwFVec2 currentpos = m_xViewport.GetCenterPosition();
		CIwFVec2 diffpos = m_xFocusPoint - currentpos;
		if (std::fabs(diffpos.x) > threshold || std::fabs(diffpos.y) > threshold) {
			m_xViewport.SetCenterPosition(currentpos + (factor * diffpos));
		}
	}
	
	// effect
	m_xEffect.Update(screensize, timestep);
	if (m_xEffect.IsActive()) {
		CIwFVec2 currentpos = m_xViewport.GetCenterPosition();
		m_xViewport.SetCenterPosition(currentpos + m_xEffect.GetEffectOffset());
	}
}

const Viewport& Camera::GetViewport() {
	return m_xViewport;
}

float Camera::CalcW2SFactorFillScreen(const CIwFVec2& worldsize, const CIwSVec2& screensize, float maxvisibleworldsize) {
	float xfactor = (float)screensize.x / std::min<float>(worldsize.x, maxvisibleworldsize);
	float yfactor = (float)screensize.y / std::min<float>(worldsize.y, maxvisibleworldsize);
	return	std::max<float>(
			xfactor < 0.0f ? -xfactor : xfactor, 
			yfactor < 0.0f ? -yfactor : yfactor);
}

float Camera::CalcW2SFactorFitInScreen(const CIwFVec2& worldsize, const CIwSVec2& screensize) {
	float xfactor = (float)screensize.x / worldsize.x;
	float yfactor = (float)screensize.y / worldsize.y;
	return	std::min<float>(
			xfactor < 0.0f ? -xfactor : xfactor, 
			yfactor < 0.0f ? -yfactor : yfactor);
}

void Camera::StartQuakeEffect(float amplitude, uint16 duration) {
	m_xEffect.StartQuake(amplitude, duration);
}

