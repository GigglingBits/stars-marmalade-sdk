#include "StarState.h"
#include "InputManager.h"
#include "SoundEngine.h"
#include "Buff.h"

/****
 * Idle state
 **/
void Star::IdleState::Initialize() {
	m_rxContext.SetTextureFrame("happy"); 
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


/****
 * Jumping state
 **/
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


/****
 * Smashing state
 **/ 
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
