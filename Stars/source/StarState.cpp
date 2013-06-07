#include "StarState.h"
#include "InputManager.h"
#include "SoundEngine.h"
#include "Buff.h"

/////////////////////////////////////////////////////////////
// Idle
/////////////////////////////////////////////////////////////
void Star::IdleState::Initialize() {
	m_rxContext.SetTextureFrame("happy");
	
	CIwFVec2 parkposition = m_rxContext.GetDragTarget();
	parkposition.x = m_rxContext.m_fAnchorLine;
	m_rxContext.MoveDragging(parkposition);
}

void Star::IdleState::Collide(Body& body) {
	m_rxContext.ShowEffect("smoke_jump");

	if (body.GetTypeName() == Buff::TypeName()) {
		body.GetHealthManager().Kill();
		SoundEngine::PlaySoundEffect("NinjaWaka");

	} else {
		body.GetHealthManager().Injure(5.0f); // just arbitrarily 5... for testing
		m_rxContext.SetTextureFrame("hurt");
		SoundEngine::PlaySoundEffect("NinjaHurt");
	}
}

void Star::IdleState::FollowPath() {
	m_rxContext.SetState(new FollowPathState(m_rxContext));
}

/////////////////////////////////////////////////////////////
// Follow path
/////////////////////////////////////////////////////////////
void Star::FollowPathState::Initialize() {
	m_rxContext.SetTextureFrame("followpath");
}

void Star::FollowPathState::Update(uint16 timestep) {
	std::queue<CIwFVec2>& path = m_rxContext.m_xPath;
	if (path.empty()) {
		m_rxContext.SetState(new IdleState(m_rxContext));
		return;
	}
	
	// distance to be travelled during this frame
	const float velocity = 10.0f; // m/s
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
}


/////////////////////////////////////////////////////////////
// Jumping
/////////////////////////////////////////////////////////////
void Star::JumpingState::Initialize() {
	m_rxContext.SetTextureFrame("jump"); 	
}

void Star::JumpingState::Attack() {
	CIwFVec2 impulse(0.0f, -40.0f);
	m_rxContext.SetImpulse(impulse);
	SoundEngine::PlaySoundEffect("NinjaAttack");
	m_rxContext.SetState(new SmashingState(m_rxContext));
}

void Star::JumpingState::Collide(Body& body) {
	m_rxContext.ShowEffect("smoke_plume");
	m_rxContext.SetState(new IdleState(m_rxContext));
}

void Star::JumpingState::Update(uint16 timestep) {
	if (!m_rxContext.GetBody().IsAwake()) {
		m_rxContext.SetState(new IdleState(m_rxContext));
	}
}


/////////////////////////////////////////////////////////////
// Smashing
/////////////////////////////////////////////////////////////
void Star::SmashingState::Initialize() {
	m_rxContext.SetTextureFrame("smash"); 	
}

void Star::SmashingState::Collide(Body& body) {
    m_rxContext.SetState(new IdleState(m_rxContext));
}

void Star::SmashingState::Update(uint16 timestep) {
	// state transition
	if (!m_rxContext.GetBody().IsAwake()) {
		m_rxContext.SetState(new IdleState(m_rxContext));
	}
}
