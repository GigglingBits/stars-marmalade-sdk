#include "Enemy.h"
#include "Debug.h"
#include "FactoryManager.h"

Enemy::Enemy(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef)
	: CompositeBody(id, bodydef, fixturedef, texturedef), m_pxParticles(NULL), m_bKnockedOut(false) {
	SetGravityScale(0.0f);
}

const char* Enemy::GetTypeName() {
	return Enemy::TypeName();
}

const char* Enemy::TypeName() {
	static const char* type = "enemy";
	return type;
}

void Enemy::KnockOut() {
	EnableCollisions(false);
	EnableRotation(true);
	
	CIwFVec2 impulse(0.0f, 2.0f);
	impulse *= GetMass();
	SetImpulse(impulse, 5.0f);
	
	SetGravityScale(1.0f);
	
	m_bKnockedOut = true;
}

void Enemy::OnColliding(Body& body) {
	if (!m_bKnockedOut && dynamic_cast<Enemy*>(&body)) {
		// any collision with another enemy leads to destruction
		KnockOut();
		ShowEffect("star_collision");
	}
	
	CompositeBody::OnColliding(body);
}

void Enemy::OnChildColliding(Body& child, Body& body) {
	if (!m_bKnockedOut && dynamic_cast<Star*>(&body)) {
		KnockOut();
		ShowEffect("star_collision");
	
		// todo: not all colisions should lead to buff emission
		EmitBuff();
	}
	
	CompositeBody::OnChildColliding(child, body);
}

void Enemy::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;
	CompositeBody::OnUpdate(frame);
	
	if (!m_pxParticles) {
		m_pxParticles = new ParticleSystem(FactoryManager::GetTextureFactory().GetConfig("particle_white_star"), CIwFVec2(0.0f, 0.0f), "", "");
		m_pxParticles->SetPosition(GetPosition());
		m_pxParticles->SetBirthRate(3);
		m_pxParticles->SetParticleSize(CIwFVec2(0.175f, 0.175f));
		m_pxParticles->SetParticleSpeed(CIwFVec2(5.0f, 5.0f));
		m_pxParticles->Start();
	} else {
		m_pxParticles->SetPosition(GetPosition());
		m_pxParticles->Update(frame);
	}
}

void Enemy::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;
	if (m_pxParticles) {
		m_pxParticles->Render(renderer, frame);
	}
	CompositeBody::OnRender(renderer, frame);
}
