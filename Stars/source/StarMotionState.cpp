#include "Main.h"
#include "StarMotionState.h"
#include "InputManager.h"
#include "SoundEngine.h"
#include "Nugget.h"
#include "GameFoundation.h"
#include "Debug.h"

/////////////////////////////////////////////////////////////
// Retracting
/////////////////////////////////////////////////////////////
void Star::RetractingState::Initialize() {
	m_rxContext.EnableCollisions(true);
	m_rxContext.SetMotionTextureFrame("idle");
	m_rxContext.DisableParticles();

	m_rxContext.GetBody().SetLinearDamping(5.0f);

	CIwFVec2 dragtarget = m_rxContext.GetDragTarget();
	dragtarget.x = m_rxContext.m_fAnchorLine;
	m_rxContext.MoveDragging(dragtarget);
	
	m_rxContext.m_bAutoOrient = false;
}

void Star::RetractingState::FollowPath() {
	m_rxContext.SetState(new FollowState(m_rxContext));
}

void Star::RetractingState::Collide(Body& body) {
	IW_CALLSTACK_SELF;
	if (Nugget* nugget = dynamic_cast<Nugget*>(&body)) {
		SoundEngine::GetInstance().PlaySoundEffect("EatNugget");
		
		DustEventArgs args;
		args.EventType = eDustEventTypeCollectSingle;
		args.amount = nugget->GetDustAmount();
		args.position = nugget->GetPosition();
		m_rxContext.DustEvent.Invoke(m_rxContext, args);
	} else {
		m_rxContext.SetTextureFrame("hurt");
		SoundEngine::GetInstance().PlaySoundEffect("Ouch");
		
		DustEventArgs args;
		args.EventType = eDustEventTypeRollback;
		args.position = body.GetPosition();
		m_rxContext.DustEvent.Invoke(m_rxContext, args);
		
		m_rxContext.ClearPath();
		m_rxContext.SetState(new RecoverState(m_rxContext));
	}
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
	m_rxContext.EnableCollisions(true);
	m_rxContext.SetMotionTextureFrame("followpath");
	m_rxContext.EnableParticles();
	m_rxContext.GetBody().SetLinearDamping(0.1f);
	
	m_rxContext.m_bAutoOrient = true;
}

void Star::FollowState::FollowPath() {
	m_rxContext.SetState(new FollowState(m_rxContext));
}

void Star::FollowState::Collide(Body& body) {
	IW_CALLSTACK_SELF;
	
	if (Nugget* nugget = dynamic_cast<Nugget*>(&body)) {
		SoundEngine::GetInstance().PlaySoundEffect("EatNugget");
		
		DustEventArgs args;
		args.EventType = eDustEventTypeCollect;
		args.amount = nugget->GetDustAmount();
		args.position = nugget->GetPosition();
		m_rxContext.DustEvent.Invoke(m_rxContext, args);
	} else {
		m_rxContext.SetTextureFrame("hurt");
		SoundEngine::GetInstance().PlaySoundEffect("Ouch");
		
		DustEventArgs args;
		args.EventType = eDustEventTypeRollback;
		args.position = body.GetPosition();
		m_rxContext.DustEvent.Invoke(m_rxContext, args);
		
		m_rxContext.ClearPath();
		m_rxContext.SetState(new RecoverState(m_rxContext));
	}
}

void Star::FollowState::Update(uint16 timestep) {
	// end condition
	std::queue<CIwFVec2>& path = m_rxContext.m_xPath;
	if (path.empty()) {
		// transition to next state
		DustEventArgs args;
		args.EventType = eDustEventTypeCommit;
		args.position = m_rxContext.GetPosition();
		m_rxContext.DustEvent.Invoke(m_rxContext, args);
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

/////////////////////////////////////////////////////////////
// Recover
/////////////////////////////////////////////////////////////
void Star::RecoverState::Initialize() {
	m_rxContext.EnableCollisions(false);
	m_rxContext.SetMotionTextureFrame("recover");
	m_rxContext.DisableParticles();

	m_rxContext.GetBody().SetLinearDamping(5.0f);

	CIwFVec2 dragtarget = m_rxContext.GetDragTarget();
	dragtarget.x = m_rxContext.m_fAnchorLine;
	m_rxContext.MoveDragging(dragtarget);
	
	m_uiRemainingTime = 2000; // sleep for 2 seconds
	
	m_rxContext.m_bAutoOrient = true;
}

void Star::RecoverState::FollowPath() {
	m_rxContext.SetState(new FollowState(m_rxContext));
}

void Star::RecoverState::Update(uint16 timestep) {
	// balance the drag force
	if (m_rxContext.IsDragging()) {
		float distance = (m_rxContext.GetDragTarget() - m_rxContext.GetPosition()).GetLength();
		float force = (distance > 1.0f ? 1.0f + (distance / 10.0f) : distance) * 5.0f * m_rxContext.GetMass();
		m_rxContext.SetDragForce(force);
	}

	// is recovery time over?
	if (timestep < m_uiRemainingTime) {
		m_uiRemainingTime -= timestep;
	} else {
		m_rxContext.SetState(new RetractingState(m_rxContext));
	}
}
