#include "StarMotionState.h"
#include "InputManager.h"
#include "SoundEngine.h"
#include "Buff.h"

/////////////////////////////////////////////////////////////
// Idle
/////////////////////////////////////////////////////////////
void Star::RetractingState::Initialize() {
	m_rxContext.ShowTextEffect("Retracting");
	m_rxContext.SetMotionTextureFrame("happy");
}

void Star::RetractingState::FollowPath() {
	m_rxContext.SetState(new FollowState(m_rxContext));
}

void Star::RetractingState::Update(uint16 timestep) {
	// distance to be travelled during this frame
	const float velocity = m_rxContext.m_fPathSpeed / 2.0f; // m/s
	float framedistance = velocity * ((float)timestep / 1000.0f);
	
	// identify the point on the path
	CIwFVec2 dragtarget = m_rxContext.GetDragTarget();
	CIwFVec2 parkposition = dragtarget;
	parkposition.x = m_rxContext.m_fAnchorLine;
	CIwFVec2 step = parkposition - dragtarget;
	
	float stepdistance = step.GetLength();
	if (stepdistance <= 0.0f) {
		return;
	} else if (framedistance >= stepdistance) {
		dragtarget = parkposition;
	} else {
		dragtarget += step * (framedistance / stepdistance);
	}
	
	// move to new place
	m_rxContext.MoveDragging(dragtarget);
}


/////////////////////////////////////////////////////////////
// Follow
/////////////////////////////////////////////////////////////
void Star::FollowState::Initialize() {
	m_rxContext.ShowTextEffect("Following");
	m_rxContext.SetMotionTextureFrame("followpath");
}

void Star::FollowState::Update(uint16 timestep) {
	std::queue<CIwFVec2>& path = m_rxContext.m_xPath;
	if (path.empty()) {
		m_rxContext.SetState(new RetractingState(m_rxContext));
		return;
	}
	
	// distance to be travelled during this frame
	const float velocity = m_rxContext.m_fPathSpeed; // m/s
	float framedistance = velocity * ((float)timestep / 1000.0f);
	
	// identify the point on the path
	CIwFVec2 dragtarget = m_rxContext.GetDragTarget();
	while (path.empty() || framedistance > 0.0f) {
		CIwFVec2 step = path.front() - dragtarget;
		float stepdistance = step.GetLength();
		if (stepdistance <= 0.0f) {
			path.pop();
		} else if (framedistance >= stepdistance) {
			framedistance -= stepdistance;
			dragtarget = path.front();
			path.pop();
		} else {
			dragtarget += step * (framedistance / stepdistance);
			break;
		}
	}
	
	// move to new place
	m_rxContext.MoveDragging(dragtarget);
	
	// draw effect
	m_rxContext.ShowEffect("smoke_plume");
}