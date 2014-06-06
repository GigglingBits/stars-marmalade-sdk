#ifndef __PATHTRACKER_H__
#define __PATHTRACKER_H__

#include <vector>
#include "Renderable.h"
#include "IwGeomFVec2.h"
#include "PathRecorder.h"
#include "s3e.h"

#define PATHTRACKER_MAX_PATH_POINTS		200
#define PATHTRACKER_STEP_LENGTH			0.5f
#define PATHTRACKER_POINT_SIZE			0.15f

class PathTracker : public Renderable {
private:
	// actual path
	CIwFVec2 m_axPath[PATHTRACKER_MAX_PATH_POINTS];
	uint16 m_uiPointCount;
	
	// tracking
	float m_fPosition;
	
	// rendering
	Texture* m_pxTexture;
	CIwFVec2 m_axXY[4 * PATHTRACKER_MAX_PATH_POINTS];
	CIwFVec2 m_axUV[4 * PATHTRACKER_MAX_PATH_POINTS];
	uint32 m_auiCols[4 * PATHTRACKER_MAX_PATH_POINTS];
	
public:
	PathTracker();

	void InitializeTexture();

	void ImportPath(const std::vector<CIwFVec2>& path, float leadindistance = 0.0f);
	void ClearPath();

	bool Walk(float distance);
	bool IsWalking();
	CIwFVec2 GetWalkingPosition();
	
protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);

private:
	bool Append(const CIwFVec2& point);

	int GetPositionIndex();
	
	void InitializeXY();
	void InitializeUV();
	void InitializeCols(float leadindistance = 0.0f);
};

#endif
