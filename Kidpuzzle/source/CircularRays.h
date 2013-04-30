#ifndef __CIRCULARRAYS_H__
#define __CIRCULARRAYS_H__

#include "Renderable.h"

#define CIRCULARRAYS_RAY_COUNT 12

class CircularRays : public Renderable {
private:
	CIwFVec2 m_xPosition;

	bool m_bCacheInvalidated;
	CIwFVec2 m_axRayVertsCache[CIRCULARRAYS_RAY_COUNT * 2];
	CIwFVec2 m_axTriangleVertsCache[3];

public:
	CircularRays();
	void SetPosition(const CIwFVec2& pos);

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
