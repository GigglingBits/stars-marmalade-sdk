#include "CircularRays.h"
#include <math.h>
#include "Debug.h"

CircularRays::CircularRays() {
	SetRenderingLayer(Renderer::eRenderingLayerGameStaticObjects);
	m_xPosition = CIwFVec2::g_Zero;
	m_bCacheInvalidated = true;
}

void CircularRays::SetPosition(const CIwFVec2& pos) {
	if (pos != m_xPosition) {
		m_xPosition = pos;
		m_bCacheInvalidated = true;
	}
}

void CircularRays::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	// check geometry
	static const int circlevertcount = CIRCULARRAYS_RAY_COUNT * 2;

	// generate verts, if needed
	static const float raylength = 5.0f;
	if (m_bCacheInvalidated) {
		for (int i = 0; i < circlevertcount; i++) {
			float angle = 2.0f * (float)M_PI * (float)i / (float)circlevertcount;
			float c = cosf(angle);
			float s = sinf(angle);
			m_axRayVertsCache[i].x = m_xPosition.x + (s * raylength);
			m_axRayVertsCache[i].y = m_xPosition.y + (c * raylength);
		}
		m_axTriangleVertsCache[0] = m_xPosition;
		m_bCacheInvalidated = false;
	}
}

void CircularRays::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (m_bCacheInvalidated) {
		return;
	}

	// check geometry
	static const int trianglevertcount = sizeof(m_axTriangleVertsCache) / sizeof(CIwFVec2);
	IwAssertMsg(MYAPP, trianglevertcount == 3, ("Cache must have a length of 3"));
	
	// draw the triangles	
	static const uint32 cols[trianglevertcount] = {
		0x33ffbbbb, 
		0x00ffffff, 
		0x00ffffff 
	};
	
	for (int i = 0; i < CIRCULARRAYS_RAY_COUNT; i++) {
		m_axTriangleVertsCache[1] = m_axRayVertsCache[i * 2];
		m_axTriangleVertsCache[2] = m_axRayVertsCache[i * 2 + 1];
		renderer.DrawPolygon(m_axTriangleVertsCache, trianglevertcount, NULL, (uint32*)cols);
	}
}

