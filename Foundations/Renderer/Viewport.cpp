#include <string>
#include <math.h>
#include "Viewport.h"

Viewport::Viewport() {
	// virtual camera
	m_xViewportWorldPos = CIwFVec2::g_Zero;
	m_xViewportScreenPos = CIwSVec2::g_Zero;
	m_xViewportScreenSize = CIwSVec2::g_Zero;			// view size in pixels
	m_fWorldToScreenFactor = 50.0;				// 1m -> 50px

	UpdateWorldSizePx();
	UpdateViewportScreenPos();
}

void Viewport::SetGeometry(const CIwFVec2& worldsize, const CIwSVec2& viewportsize) {
	if (m_xWorldSize == worldsize && m_xViewportScreenSize == viewportsize) {
		return;
	}

	m_xWorldSize = worldsize;
	m_xViewportScreenSize = viewportsize;

	UpdateWorldSizePx();
	UpdateViewportScreenPos();
}

void Viewport::SetWorldToScreenFactor(float value) {
	if (m_fWorldToScreenFactor == value) {
		return;
	}

	m_fWorldToScreenFactor = value;

	UpdateWorldSizePx();
	UpdateViewportScreenPos();
}

float Viewport::GetWorldToScreenFactor() const{
	return m_fWorldToScreenFactor;
}

void Viewport::SetCenterPosition(const CIwFVec2& point) {
	if (m_xViewportWorldPos == point) {
		return;
	}

	m_xViewportWorldPos = point;
	UpdateViewportScreenPos();
}

const CIwFVec2& Viewport::GetCenterPosition() const {
	return m_xViewportWorldPos;
}

CIwSVec2 Viewport::GetScreenViewOffset() const {
	return -m_xViewportScreenPos;
}

const CIwSVec2& Viewport::GetViewportSize() const {
	return m_xViewportScreenSize;
}

void Viewport::UpdateWorldSizePx() {
	m_xWorldSizePx.x = (int16)(m_xWorldSize.x * m_fWorldToScreenFactor);
	m_xWorldSizePx.y = (int16)(m_xWorldSize.y * m_fWorldToScreenFactor);
}

void Viewport::UpdateViewportScreenPos() {
	m_xViewportScreenPos = WorldToScreen(m_xViewportWorldPos) - (m_xViewportScreenSize / IW_SFIXED(2));
}

CIwSVec2 Viewport::WorldToScreen(const CIwFVec2& worldpoint) const {
	CIwSVec2 screenpoint;

	// scaling
	screenpoint.x = (int16)(m_fWorldToScreenFactor * worldpoint.x);
	screenpoint.y = (int16)(m_fWorldToScreenFactor * worldpoint.y);

	// convert coord space (screen coords are upside down) 
	screenpoint.y = m_xWorldSizePx.y - screenpoint.y;

	return screenpoint;
}

CIwFVec2 Viewport::ScreenToWorld(const CIwSVec2& screenpoint) const {
	CIwFVec2 worldpoint;

	// convert coord space (screen coords are upside down) 
	int16 y = m_xWorldSizePx.y - screenpoint.y;

	// scaling
	worldpoint.x = (float)screenpoint.x / m_fWorldToScreenFactor;
	worldpoint.y = (float)y / m_fWorldToScreenFactor;
	
	return worldpoint;
}

CIwSVec2 Viewport::ScreenToView(const CIwSVec2& screenpoint) const {
	return screenpoint + GetScreenViewOffset();
}

CIwSVec2 Viewport::ViewToScreen(const CIwSVec2& viewpoint) const {
	return viewpoint - GetScreenViewOffset();
}

CIwSVec2 Viewport::WorldToView(const CIwFVec2& worldpoint) const {
	return ScreenToView(WorldToScreen(worldpoint));
}

CIwFVec2 Viewport::ViewToWorld(const CIwSVec2& viewpoint) const {
	return ScreenToWorld(ViewToScreen(viewpoint));
}

Viewport Viewport::operator=(const Viewport& viewport) {
	this->m_xWorldSize = viewport.m_xWorldSize;
	this->m_xWorldSizePx = viewport.m_xWorldSizePx;

	this->m_xViewportScreenSize = viewport.m_xViewportScreenSize;
	this->m_xViewportWorldPos = viewport.m_xViewportWorldPos;
	this->m_xViewportScreenPos = viewport.m_xViewportScreenPos;
	this->m_fWorldToScreenFactor = viewport.m_fWorldToScreenFactor;

	return *this;
}
