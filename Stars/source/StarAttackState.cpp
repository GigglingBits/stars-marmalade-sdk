#include "GameFoundation.h"
#include "StarAttackState.h"
#include "InputManager.h"
#include "SoundEngine.h"
#include "Buff.h"

/////////////////////////////////////////////////////////////
// Peaceful
/////////////////////////////////////////////////////////////
void Star::PeacefulState::Initialize() {
	m_rxContext.ShowTextEffect("Peaceful");
	m_rxContext.ClearAttackTextureFrame();
}

void Star::PeacefulState::BeginBlock() {
	m_rxContext.SetState(new BlockState(m_rxContext));
}

void Star::PeacefulState::BeginAttack() {
	m_rxContext.SetState(new AttackState(m_rxContext));
}

void Star::PeacefulState::Collide(Body& body) {
	if (body.GetTypeName() == Buff::TypeName()) {
		body.GetHealthManager().Kill();
		body.ShowEffect("star_collision");
		SoundEngine::GetInstance().PlaySoundEffect("EatNugget");

	} else {
		body.GetHealthManager().Injure(5.0f); // just arbitrarily 5... for testing
		m_rxContext.SetTextureFrame("hurt");
		SoundEngine::GetInstance().PlaySoundEffect("Ouch");
		
		if (GameFoundation* game = m_rxContext.GetGameFoundation()) {
			game->CancelDust(m_rxContext.GetPosition());
		}
	}
}

void Star::PeacefulState::Update(uint16 timestep) {
}

/////////////////////////////////////////////////////////////
// Block
/////////////////////////////////////////////////////////////
void Star::BlockState::Initialize() {
	m_rxContext.ShowTextEffect("Blocking");
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
	m_rxContext.ShowTextEffect("Attacking");
	m_rxContext.SetAttackTextureFrame("attack");
}

void Star::AttackState::EndAttack() {
	m_rxContext.SetState(new PeacefulState(m_rxContext));
}

void Star::AttackState::Update(uint16 timestep) {
}
