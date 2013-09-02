#include "Trajectory.h"
#include "Debug.h"

Trajectory::Trajectory(GameFoundation& game) : m_rxGame(game) {
	m_xPosition = CIwFVec2::g_Zero;
	m_xVelocity = CIwFVec2::g_Zero;
	m_xGravity = game.GetGravity();

	SetRederingLayer(Renderer::eRenderingLayerGameStaticObjects);

	m_iVisibleStepCount = 0;
	m_bCacheExpired = true;
}

void Trajectory::SetPosition(const CIwFVec2& worldpos) {
	m_bCacheExpired = worldpos != m_xPosition;
	m_xPosition = worldpos;
}

void Trajectory::SetVelocity(const CIwFVec2& impulse, float mass){
	CIwFVec2 velocity = impulse / mass;
	m_bCacheExpired = velocity != m_xVelocity;
	m_xVelocity = velocity;
}

void Trajectory::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (m_xVelocity.IsZero()) {
		m_iVisibleStepCount = 0;
		return;
	}

	if (m_bCacheExpired) {
		float avgframetime = frame.GetAvgSimulatedDurationMs();

		int stepcount = (int) (1000 / avgframetime);	// calculate trajectory for 1 second
		stepcount = std::min<int>(stepcount, TRAJECTORY_MAX_STEPS);
		stepcount = std::max<int>(stepcount, TRAJECTORY_MIN_STEPS);

		UpdateColourCache(stepcount);
		UpdateVertexCache(m_xPosition, m_xVelocity, avgframetime, stepcount);
		m_bCacheExpired = false;
	}
}

void Trajectory::UpdateVertexCache(const CIwFVec2& startpos, const CIwFVec2& velocity, float avgframetime, int totalstepcount) {
	float avgframetimesec = avgframetime / 1000.0f;

	// re-calculate the trajectory
	for (int i = 0; i < totalstepcount; i++) {
		m_axVertexCache[i] = CalculateTrajectoryPoint(
			m_xPosition, m_xVelocity, 
			avgframetimesec, 
			(float)i);
	}

	// see of the trajectory causes a collision
	m_iVisibleStepCount = totalstepcount;
	for (int i = 1; i < totalstepcount - 1; i++) {
		if (m_rxGame.RayHitTest(m_axVertexCache[i - 1], m_axVertexCache[i])) {
			// colision!
			m_iVisibleStepCount = i;
			break;
		}
	}
}

void Trajectory::UpdateColourCache(int totalstepcount) {
	// prevent buffer overflow
	int stepcount = totalstepcount > TRAJECTORY_MAX_STEPS ? TRAJECTORY_MAX_STEPS : totalstepcount;

	// separate alpha from colour channels
	uint32 purealpha = TRAJECTORY_LINE_COLOUR & 0xff000000;
	uint32 purecolour = TRAJECTORY_LINE_COLOUR & 0x00ffffff;

	// the alpha decreases for every step
	for (int i = 0; i < stepcount; i++) {
		uint32 newalpha = (uint32)(purealpha * ((stepcount - i) / (float) stepcount));
		uint32 newcolour = (newalpha & 0xff000000) | purecolour;
		m_aiColourCache[i] = newcolour;
	}
}

CIwFVec2 Trajectory::CalculateTrajectoryPoint(const CIwFVec2& startpos, const CIwFVec2& velocity, float avgframetime, float stepnumber) {
	// idea from: http://www.iforce2d.net/b2dtut/projected-trajectory

	// avgframetime is given in seconds; not milliseconds

	//velocity and gravity are given per second but we want time step values here
	//float t = 1 / 60.0f; // seconds per time step (at 60fps)

	CIwFVec2 stepVelocity = avgframetime * velocity; // m/s
	CIwFVec2 stepGravity = avgframetime * avgframetime * m_xGravity; // m/s/s
  
	return startpos + stepnumber * stepVelocity + 0.5f * (stepnumber * stepnumber + stepnumber) * stepGravity;
}

void Trajectory::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (m_iVisibleStepCount > 1) {	
		//// line width: http://stackoverflow.com/questions/3516757/the-proper-way-to-save-restore-opengl-es-state
		//GLfloat savedLineWidth = 1.0f;
		//glGetFloatv(GL_LINE_WIDTH, &savedLineWidth);
		//glLineWidth(TRAJECTORY_LINE_WIDTH);
		renderer.DrawLine(m_axVertexCache, m_iVisibleStepCount, m_aiColourCache);
		//IwGxFlush();
		//glLineWidth(savedLineWidth);
	}
}		
