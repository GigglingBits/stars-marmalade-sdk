#include "StarMotionState.h"
#include "InputManager.h"
#include "SoundEngine.h"
#include "Nugget.h"
#include "GameFoundation.h"

/////////////////////////////////////////////////////////////
// Retracting
/////////////////////////////////////////////////////////////
void Star::RetractingState::Initialize() {
	m_rxContext.SetMotionTextureFrame("idle");
	m_rxContext.DisableParticles();

	CIwFVec2 dragtarget = m_rxContext.GetDragTarget();
	dragtarget.x = m_rxContext.m_fAnchorLine;
	m_rxContext.MoveDragging(dragtarget);

	m_rxContext.GetBody().SetLinearDamping(5.0f);
}

void Star::RetractingState::FollowPath() {
	m_rxContext.SetState(new FollowState(m_rxContext));
}

void Star::RetractingState::Update(uint16 timestep) {
	// balance the drag force
	if (m_rxContext.IsDragging()) {
		float distance = (m_rxContext.GetDragTarget() - m_rxContext.GetPosition()).GetLength();
		float force = (distance > 1.0f ? 1.0f + (distance / 10.0f) : distance) * 5.0f * m_rxContext.GetMass();
		m_rxContext.SetDragForce(force);
	}
}


/////////////////////////////////////////////////////////////
// Follow
/////////////////////////////////////////////////////////////
void Star::FollowState::Initialize() {
	m_rxContext.SetMotionTextureFrame("followpath");
	m_rxContext.EnableParticles();
	m_rxContext.GetBody().SetLinearDamping(0.1f);
}

void Star::FollowState::FollowPath() {
	m_rxContext.SetState(new FollowState(m_rxContext));
}

void Star::FollowState::IncrementMultiplier() {
	if (GameFoundation* game = m_rxContext.GetGameFoundation()) {
		game->QueueDust(m_rxContext.GetPosition(), 10);
	}
}

void Star::FollowState::Update(uint16 timestep) {
	// end condition
	std::queue<CIwFVec2>& path = m_rxContext.m_xPath;
	if (path.empty()) {
		// reset the multiplier
		if (GameFoundation* game = m_rxContext.GetGameFoundation()) {
			game->CommitDust(m_rxContext.GetPosition());
		}

		// transition to next state
		m_rxContext.SetState(new RetractingState(m_rxContext));
		return;
	}
	
	// calculate distance that can be travelled during this frame
	const float velocity = m_rxContext.m_fPathSpeed; // m/s
	float framedistance = velocity * ((float)timestep / 1000.0f);
	
	// identify the next point on the path
	CIwFVec2 dragtarget = m_rxContext.GetDragTarget();
	while (!path.empty() && framedistance > 0.0f) {
		CIwFVec2 step = path.front() - dragtarget;
		float stepdistance = step.GetLength();
		if (stepdistance <= 0.0f) {
			// nothing to move on this step; proceed
			path.pop();
		} else if (framedistance >= stepdistance) {
			// move to end of current line, and proceed
			framedistance -= stepdistance;
			dragtarget = path.front();
			path.pop();
		} else {
			// cannot move longer during this frame; quit
			dragtarget += step * (framedistance / stepdistance);
			break;
		}
	}
	
	// balance the drag force
	if (m_rxContext.IsDragging()) {
		float distance = (m_rxContext.GetDragTarget() - m_rxContext.GetPosition()).GetLength();
		m_rxContext.SetDragForce(distance * 100.0f * m_rxContext.GetMass());
	}
	
	// move particles to new place
	m_rxContext.MoveDragging(dragtarget);
	if (m_rxContext.m_pxParticles) {
		m_rxContext.m_pxParticles->SetPosition(m_rxContext.GetPosition());
	}	
}
