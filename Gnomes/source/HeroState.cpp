#include "HeroState.h"
#include "InputManager.h"
#include "SoundEngine.h"

/****
 * Idle state
 **/
void Hero::IdleState::Initialize() {
	m_rxContext.SetTextureFrame("idle"); 
}

void Hero::IdleState::Jump(const CIwFVec2& impulse) {
	m_rxContext.SetImpulse(impulse);
	SoundEngine::PlaySoundEffect("NinjaHo");
	m_rxContext.ShowEffect("smoke_jump");
	m_rxContext.SetState(new JumpingState(m_rxContext));
}


/****
 * Jumping state
 **/
void Hero::JumpingState::Initialize() {
	m_rxContext.SetTextureFrame("jump"); 	
}

void Hero::JumpingState::Attack() {
	CIwFVec2 impulse(0.0f, -40.0f);
	m_rxContext.SetImpulse(impulse);
	SoundEngine::PlaySoundEffect("NinjaAttack");
	m_rxContext.SetState(new SmashingState(m_rxContext));
}

void Hero::JumpingState::Collide(Body& body) {
	m_rxContext.ShowEffect("smoke_plume");
	m_rxContext.SetState(new IdleState(m_rxContext));
}

void Hero::JumpingState::Update(uint16 timestep) {
	if (!m_rxContext.GetBody().IsAwake()) {
		m_rxContext.SetState(new IdleState(m_rxContext));
	}
}


/****
 * Smashing state
 **/ 
void Hero::SmashingState::Initialize() {
	m_rxContext.SetTextureFrame("smash"); 	
}

void Hero::SmashingState::Collide(Body& body) {
    m_rxContext.SetState(new IdleState(m_rxContext));
}

void Hero::SmashingState::Update(uint16 timestep) {
	// state transition
	if (!m_rxContext.GetBody().IsAwake()) {
		m_rxContext.SetState(new IdleState(m_rxContext));
	}
}
