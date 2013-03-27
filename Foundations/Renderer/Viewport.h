#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

#include "IwGeom.h"

class Viewport {
private:
	CIwSVec2 m_xViewportScreenSize;	// size of visible area in pixels
	CIwSVec2 m_xViewportScreenPos;	// position of visible area at its origin
	CIwFVec2 m_xViewportWorldPos;	// position of visible area at the center of the viewport

	float m_fWorldToScreenFactor;	// actual zoom factor (world space to screen/view space)

	CIwFVec2 m_xWorldSize;			// size of world in meters
	CIwSVec2 m_xWorldSizePx;		// size of world in pixels

public:
	Viewport();

	// geometry
	void SetGeometry(const CIwFVec2& worldsize, const CIwSVec2& viewportsize);
	const CIwSVec2& GetViewportSize() const;

	// zooming
	void SetWorldToScreenFactor(float value);
	float GetWorldToScreenFactor() const;

	// positioning
	void SetCenterPosition(const CIwFVec2& point);
 	const CIwFVec2& GetCenterPosition() const;
	CIwSVec2 GetScreenViewOffset() const;

	// conversions:
	// world:	the simulated area
	// screen:	area where the world is rendered to (not all may be visible, if offset or zoom are applied)
	// view:	visible are of the screen
	CIwSVec2 WorldToScreen(const CIwFVec2& worldpoint) const; 
	CIwFVec2 ScreenToWorld(const CIwSVec2& screenpoint) const;

	CIwSVec2 ScreenToView(const CIwSVec2& screenpoint) const;
	CIwSVec2 ViewToScreen(const CIwSVec2& viewpoint) const;

	CIwSVec2 WorldToView(const CIwFVec2& worldpoint) const;
	CIwFVec2 ViewToWorld(const CIwSVec2& viewpoint) const;

private:
	void UpdateWorldSizePx();
	void UpdateViewportScreenPos();

public:
	Viewport operator=(const Viewport& viewport);
};

#endif
