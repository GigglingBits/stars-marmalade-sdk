#include "Enemy.h"
#include "Debug.h"
#include "FactoryManager.h"

Enemy::Enemy(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef)
	: CompositeBody(id, bodydef, fixturedef, texturedef), m_pxParticles(NULL) {
}

const char* Enemy::GetTypeName() {
	return Enemy::TypeName();
}

const char* Enemy::TypeName() {
	static const char* type = "enemy";
	return type;
}

void Enemy::OnColliding(Body& thisbody, Body& otherbody) {
	IW_CALLSTACK_SELF;
	
	// any collision leads to selfdestruction
	GetHealthManager().Kill();
	ShowEffect("star_collision");
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
