#ifndef __BACKGROUNDPAINTER_H__
#define __BACKGROUNDPAINTER_H__

#include <string>
#include "Renderable.h"

// perspective correction factors
#define BG_SKY_PERSPECTIVE 0.15f
#define BG_FAR_PERSPECTIVE 0.28f
#define BG_NEAR_PERSPECTIVE 0.55f

class BackgroundPainter : public Renderable {
private:
	CIwFVec2 m_xWorldCenter; // center of world
	CIwFVec2 m_xWorldRadius; // distance vector from center to upper right corner

	CIwTexture* m_pxBackgroundSky;
	CIwTexture* m_pxBackgroundFar;
	CIwTexture* m_pxBackgroundNear;

public:
	BackgroundPainter(std::string background);

	void SetGeometry(const CIwFVec2& worldsize, const CIwSVec2& viewportsize, float margin);

private:
	void OnUpdate(const FrameData& frame);
	void OnRender(Renderer& renderer, const FrameData& frame);

	void SetVerts(const CIwFVec2& offset, float perspective, CIwFVec2 verts[4]);
};

#endif
