#ifndef __TRAJECTORY_H__
#define __TRAJECTORY_H__

#include "Renderable.h"
#include "GameFoundation.h"

#define TRAJECTORY_MAX_STEPS (1 * 60)		// Flying for 1 second, but max 60 per steps second
#define TRAJECTORY_MIN_STEPS (1 * 10)		// Flying for 1 second, but min 10 per steps second

#define TRAJECTORY_LINE_COLOUR 0xffffffff
#define TRAJECTORY_LINE_WIDTH 10.0f

class Trajectory : public Renderable {
private:
	GameFoundation& m_rxGame;

	bool m_bCacheExpired;
	CIwFVec2 m_axVertexCache[TRAJECTORY_MAX_STEPS];
	uint32 m_aiColourCache[TRAJECTORY_MAX_STEPS];

	int m_iVisibleStepCount;

	CIwFVec2 m_xPosition;
	CIwFVec2 m_xVelocity;
	CIwFVec2 m_xGravity;

public:
	Trajectory(GameFoundation& game);

	void SetPosition(const CIwFVec2& worldpos);
	void SetVelocity(const CIwFVec2& impulse, float mass);

private:
	void UpdateColourCache(int totalstepcount);
	void UpdateVertexCache(const CIwFVec2& startpos, const CIwFVec2& velocity, float avgtimestep, int totalstepcount);

	CIwFVec2 CalculateTrajectoryPoint(const CIwFVec2& startpos, const CIwFVec2& velocity, float avgframetime, float stepnumber);

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
