#include "PathTracker.h"
#include "FactoryManager.h"
#include "Debug.h"

PathTracker::PathTracker() : m_uiPointCount(0), m_pxTexture(NULL) {
	InitializeUV();
	InitializeCols();
}

void PathTracker::Clear() {
	m_uiPointCount = 0;
}

bool PathTracker::Append(const CIwFVec2& point) {
	IW_CALLSTACK_SELF;
	if (m_uiPointCount < PATHTRACKER_MAX_PATH_POINTS) {
		m_axPath[m_uiPointCount++] = point;
		return true;
	}
	IwAssert(MYAPP, m_uiPointCount < PATHTRACKER_MAX_PATH_POINTS);
	return false;
}

void PathTracker::ImportPath(CIwFVec2* verts, int vertcount) {
	IW_CALLSTACK_SELF;
	Clear();
	
	// qualify
	if (vertcount < 2) {
		IwAssertMsg(MYAPP, vertcount >= 2, ("Cannot import path consisting of less than 2 points."));
		return;
	}
	
	// path conditioning
	int i = 0;
	CIwFVec2 current = verts[i];
	CIwFVec2 next = verts[++i];
	Append(current);
	while (i < vertcount) {
		CIwFVec2 remaining = next - current;

		// walk along the vector as long as possible; store each step
		while (remaining.GetLength() > PATHTRACKER_STEP_LENGTH) {
			CIwFVec2 step = remaining.GetNormalised() * PATHTRACKER_STEP_LENGTH;
			current += step;
			if (!Append(current)) {
				break;
			}
			remaining -= step;
		}
		
		// too short for next step; find next vector
		if (i >= vertcount - 1) {
			// there is no next vector...
			// could consider to record the last known point: Append(next);
			break;
		} else {
			// increment, and recycle remainder
			current = next;
			next = verts[++i];
			if (current != next) {
				// extend next vector
				current -= (next - current).GetNormalised() * remaining.GetLength();
			} else {
				// cannot extend vector of 0 length; direction cannot be evaluated
				current -= remaining;
			}
		}
	}
	
	InitializeTexture();
	InitializeXY();
}

void PathTracker::InitializeTexture() {
	if (!m_pxTexture) {
		m_pxTexture = FactoryManager::GetTextureFactory().Create("path_point");
	}
}


void PathTracker::InitializeUV() {
	for (int i = 0; i < PATHTRACKER_MAX_PATH_POINTS;i++) {
		m_axUV[i*4+0] = CIwFVec2(0.0f, 0.0f);
		m_axUV[i*4+1] = CIwFVec2(1.0f, 0.0f);
		m_axUV[i*4+2] = CIwFVec2(1.0f, 1.0f);
		m_axUV[i*4+3] = CIwFVec2(0.0f, 1.0f);
	}
}

void PathTracker::InitializeCols() {
	for (int i = 0; i < PATHTRACKER_MAX_PATH_POINTS * 4;i++) {
		m_auiCols[i] = 0xffffffff;
	}
}

void PathTracker::InitializeXY() {
	const float SIZE = PATHTRACKER_POINT_SIZE / 2.0f;
	for (int i = 0; i < PATHTRACKER_MAX_PATH_POINTS;i++) {
		CIwFVec2 point = m_axPath[i];
		m_axXY[i*4+0] = point + CIwFVec2(-SIZE, -SIZE);
		m_axXY[i*4+1] = point + CIwFVec2(SIZE, -SIZE);
		m_axXY[i*4+2] = point + CIwFVec2(SIZE, SIZE);
		m_axXY[i*4+3] = point + CIwFVec2(-SIZE, SIZE);
	}
}

void PathTracker::OnUpdate(const FrameData& frame) {
	if (m_pxTexture) {
		m_pxTexture->Update(frame.GetRealDurationMs());
	}
}

void PathTracker::OnRender(Renderer& renderer, const FrameData& frame) {
	if (m_uiPointCount <= 1) {
		return;
	}
	
	if (!m_pxTexture && !m_pxTexture->IsImage()) {
		uint32 col = m_pxTexture->IsColour() ? m_pxTexture->GetColour() : 0xffffffff;
		renderer.DrawPolygon(m_axPath, m_uiPointCount, col, 0x00000000);
	} else {
		renderer.DrawImage(m_pxTexture->GetImage(), m_axXY, m_axUV, m_auiCols, m_uiPointCount * 4);
	}
}
