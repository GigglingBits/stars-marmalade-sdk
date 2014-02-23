#include "GameFoundation.h"
#include "StarAttackState.h"
#include "InputManager.h"
#include "SoundEngine.h"
#include "Nugget.h"

/////////////////////////////////////////////////////////////
// Peaceful
/////////////////////////////////////////////////////////////
void Star::PeacefulState::Initialize() {
	m_rxContext.ClearAttackTextureFrame();
}

void Star::PeacefulState::BeginBlock() {
	m_rxContext.SetState(new BlockState(m_rxContext));
}

void Star::PeacefulState::BeginAttack() {
	m_rxContext.SetState(new AttackState(m_rxContext));
}

void Star::PeacefulState::Update(uint16 timestep) {
}

/////////////////////////////////////////////////////////////
// Block
/////////////////////////////////////////////////////////////
void Star::BlockState::Initialize() {
	m_rxContext.SetAttackTextureFrame("block");
}

void Star::BlockState::EndBlock() {
	m_rxContext.SetState(new PeacefulState(m_rxContext));
}

void Star::BlockState::Update(uint16 timestep) {
}

/////////////////////////////////////////////////////////////
// Attack
/////////////////////////////////////////////////////////////
void Star::AttackState::Initialize() {
	m_rxContext.SetAttackTextureFrame("attack");
}

void Star::AttackState::EndAttack() {
	m_rxContext.SetState(new PeacefulState(m_rxContext));
}

void Star::AttackState::Update(uint16 timestep) {
}
