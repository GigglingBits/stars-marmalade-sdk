#include "Hero.h"
#include "HeroState.h"
#include "InputManager.h"

/****
 * Hero main implementation
 **/
Hero::Hero(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef) 
	: CompositeBody(id, bodydef, fixturedef, texturedef), m_pxCurrentState(NULL) {
	GetBody().SetBullet(true);
	SetState(new IdleState(*this));
}

Hero::~Hero() {
	if (m_pxCurrentState) {
		delete m_pxCurrentState;
	}
}

const char* Hero::GetTypeName() {
	return Hero::TypeName();
}

const char* Hero::TypeName() {
	static const char* type = "hero";
	return type;
}

void Hero::OnColliding(Body& thisbody, Body& otherbody) {
	GetCurrentState().Collide(otherbody);
}

void Hero::OnUpdate(const FrameData& frame) {
	CompositeBody::OnUpdate(frame);
	GetCurrentState().Update(frame.GetSimulatedDurationMs());
}

void Hero::Jump(const CIwFVec2& impulse) {
	GetCurrentState().Jump(impulse);
}

void Hero::Attack() {
	GetCurrentState().Attack();
}

void Hero::Sit() {
	GetCurrentState().Stay();
}

void Hero::SetTextureFrame(std::string id) {
	GetTexture().SelectFrame(id);
}


/****
 * State machine 
 **/
void Hero::SetState(Hero::StateBase* newstate) {
	IwAssertMsg(MYAPP, newstate, ("Empty state must not be set."));
	if (m_pxCurrentState) {
		delete m_pxCurrentState;
	}
	newstate->Initialize();
	m_pxCurrentState = newstate;
}

Hero::StateBase& Hero::GetCurrentState() {
	IwAssertMsg(MYAPP, m_pxCurrentState, ("State is empty. This is a program error."));
	return *m_pxCurrentState;
}
