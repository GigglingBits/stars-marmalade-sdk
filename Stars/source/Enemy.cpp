#include "Enemy.h"
#include "Debug.h"
#include "FactoryManager.h"

Enemy::Enemy(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef)
	: CompositeBody(id, bodydef, fixturedef, texturedef), m_pxParticles(NULL), m_bKnockedOut(false), m_bNeedDetachSledge(false) {
	AttachSledge();
}

const char* Enemy::GetTypeName() {
	return Enemy::TypeName();
}

const char* Enemy::TypeName() {
	static const char* type = "enemy";
	return type;
}

void Enemy::RegisterSoftsopt(const std::string& childid) {
	m_xSoftSpots.insert(childid);
}

void Enemy::UnregisterSoftsopt(const std::string& childid) {
	m_xSoftSpots.erase(childid);
}

void Enemy::KnockOut() {
	EnableCollisions(false);
	
	m_bNeedDetachSledge = true;
	m_bKnockedOut = true;
}

void Enemy::Shoot() {
	Enemy::KnockOut();
}

void Enemy::AttachSledge() {
	AddChild(ENEMY_SLEDGE_CHILD, ENEMY_SLEDGE_BODY);
	AddJoint(ENEMY_SLEDGE_JOINT, "", "", ENEMY_SLEDGE_CHILD, "", BodyJoint::eJointTypeWeld);
}

void Enemy::DetachSledge() {
	// RemoveJoint(ENEMY_SLEDGE_JOINT); // joint will be implicitly removed
	RemoveChild(ENEMY_SLEDGE_CHILD);
}

void Enemy::OnChildColliding(Body& child, Body& body) {
	if (!m_bKnockedOut && dynamic_cast<Star*>(&body)) {
		if (m_xSoftSpots.find(child.GetId()) != m_xSoftSpots.end()) {
			KnockOut();
			EmitBuff();
		}
		ShowEffect("star_collision");
	}
	
	CompositeBody::OnChildColliding(child, body);
}

void Enemy::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;
	CompositeBody::OnUpdate(frame);
	
	if (m_bNeedDetachSledge) {
		DetachSledge();
		m_bNeedDetachSledge = false;
	}
	
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
