#include "BackgroundPainter.h"
#include "Debug.h"
#include <cmath>

BackgroundPainter::BackgroundPainter(std::string background) {

    std::string s;
    s.assign(background);
    s.append("_sky");
	m_pxBackgroundSky = (CIwTexture*)IwGetResManager()->GetResNamed(s.c_str(), "CIwTexture");

    s.assign(background);
    s.append("_far");
    m_pxBackgroundFar = (CIwTexture*)IwGetResManager()->GetResNamed(s.c_str(), "CIwTexture");
	
    s.assign(background);
    s.append("_near");
    m_pxBackgroundNear = (CIwTexture*)IwGetResManager()->GetResNamed(s.c_str(), "CIwTexture");

	m_xWorldCenter = CIwFVec2::g_Zero;
	m_xWorldRadius = CIwFVec2::g_Zero;

	SetRenderingLayer(Renderer::eRenderingLayerBackground);
}

void BackgroundPainter::SetGeometry(const CIwFVec2& worldsize, const CIwSVec2& viewportsize, float margin) {
	// space, wich needs a background
	CIwFVec2 worldsizev(
		std::fabs(worldsize.x), 
		std::fabs(worldsize.y));

	// find geometry of bg image, so that world is 100% covered
	// while maintaining the aspect ratio of the viewport
	float aspectratio = (float)viewportsize.x / (float)viewportsize.y;
	float width = worldsizev.y * aspectratio;
	CIwFVec2 aspectcorrection;
	if (width > worldsizev.x) {
		// make wider
		aspectcorrection.x = (width - worldsizev.x) / 2.0f;							
		aspectcorrection.y = 0.0f;
	} else {
		// make taller
		aspectcorrection.x = 0.0f;
		aspectcorrection.y = ((worldsizev.x / aspectratio) - worldsizev.y) / 2.0f;	
	}

	// defining margin with viewport aspect ratio 
	CIwFVec2 marginv = CIwFVec2(margin, margin);
	if (aspectratio >= 0.0f) {
		marginv.x *= aspectratio;
	} else {
		marginv.y /= aspectratio;
	}

	// definite geometry
	m_xWorldCenter = worldsize / 2.0f;
	m_xWorldRadius = m_xWorldCenter + aspectcorrection + marginv;
}

void BackgroundPainter::SetVerts(const CIwFVec2& offset, float perspective, CIwFVec2 verts[4]) {
	// perspective corrected geometry
	CIwFVec2 center = m_xWorldCenter;
	CIwFVec2 radius = m_xWorldRadius * (1.0f + perspective);

	// perspective corrected offset
	CIwFVec2 poffset = offset * (1.0f - perspective);

	// construct corrected verts
	verts[0] = poffset + center - radius;
	verts[1] = poffset + CIwFVec2(center.x + radius.x, center.y - radius.y);
	verts[2] = poffset + center + radius;
	verts[3] = poffset + CIwFVec2(center.x - radius.x, center.y + radius.y);
}

void BackgroundPainter::OnUpdate(const FrameData& frame) {
}

void BackgroundPainter::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	const uint32 HAZE = 0x44ffffff;

	CIwFVec2 screencenter = renderer.GetScreenCenterWorldSpace();
	CIwFVec2 offset = screencenter - m_xWorldCenter;

	const int count = 4;
	CIwFVec2 verts[count];

	if (m_pxBackgroundSky) {
		SetVerts(offset, BG_SKY_PERSPECTIVE, verts);
		renderer.DrawImage(m_pxBackgroundSky, verts, count); 
		renderer.DrawPolygon(verts, count, 0x00000000, HAZE); 
	}
	if (m_pxBackgroundFar) {
		SetVerts(offset, BG_FAR_PERSPECTIVE, verts);
		renderer.DrawImage(m_pxBackgroundFar, verts, count); 
		renderer.DrawPolygon(verts, count, 0x00000000, HAZE); 
	}
	if (m_pxBackgroundNear) {
		SetVerts(offset, BG_NEAR_PERSPECTIVE, verts);
		renderer.DrawImage(m_pxBackgroundNear, verts, count); 
		renderer.DrawPolygon(verts, count, 0x00000000, HAZE); 
	}

	//renderer.DebugDrawCoords(m_xWorldCenter);
	//renderer.DebugDrawCoords(m_xWorldCenter + offset);
	//renderer.DebugDrawCoords(m_xWorldCenter + m_xWorldRadius);
	//renderer.DebugDrawCoords(m_xWorldCenter - m_xWorldRadius);
}
