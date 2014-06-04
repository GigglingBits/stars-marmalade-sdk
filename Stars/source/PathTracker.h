#ifndef __PATHTRACKER_H__
#define __PATHTRACKER_H__

#include <vector>
#include "Renderable.h"
#include "IwGeomFVec2.h"
#include "PathRecorder.h"
#include "s3e.h"

#define PATHTRACKER_MAX_PATH_POINTS		300
#define PATHTRACKER_STEP_LENGTH			0.5f
#define PATHTRACKER_POINT_SIZE			0.2f

class PathTracker : public Renderable {
private:
	CIwFVec2 m_axPath[PATHTRACKER_MAX_PATH_POINTS];
	uint16 m_uiPointCount;
	
	Texture* m_pxTexture;
	CIwFVec2 m_axXY[4 * PATHTRACKER_MAX_PATH_POINTS];
	CIwFVec2 m_axUV[4 * PATHTRACKER_MAX_PATH_POINTS];
	uint32 m_auiCols[4 * PATHTRACKER_MAX_PATH_POINTS];
	
public:
	PathTracker();
	
	void ImportPath(CIwFVec2* verts, int vertcount);
	
protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);

private:
	void Clear();
	bool Append(const CIwFVec2& point);
	
	void InitializeXY();
	void InitializeUV();
	void InitializeCols();
	void InitializeTexture();
};

#endif
