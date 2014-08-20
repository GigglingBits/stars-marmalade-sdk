#include "Nugget.h"
#include "Debug.h"
#include "FactoryManager.h"
#include "Main.h"

Nugget::Nugget(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef)
	: Body(id, bodydef, fixturedef, texturedef), m_pxParticles(NULL), m_bMagnetActive(false) {
	SetGravityScale(0.0f);
}

const char* Nugget::GetTypeName() {
	return Nugget::TypeName();
}

const char* Nugget::TypeName() {
	static const char* type = "nugget";
	return type;
}

int Nugget::GetDustAmount() {
	return GAME_POINTS_NUGGET;
}

void Nugget::SetMagnetPosition(const CIwFVec2& pos) {
	IW_CALLSTACK_SELF;
	if (!m_bMagnetActive) {
		m_bMagnetActive = true;
		IwAssert(MYAPP, !CanDrag());
		EnableDragging(true);
		BeginDragging(GetPosition(), GetMass() * 10.0f);
		MoveDragging(pos);
		GetBody().SetLinearDamping(1.0f);
	}
	IwAssert(MYAPP, CanDrag());
	MoveDragging(pos);
}

void Nugget::CancelMagnet() {
	IW_CALLSTACK_SELF;
	if (m_bMagnetActive) {
		IwAssert(MYAPP, IsDragging());
		/*
		m_bMagnetActive = false;
		EndDragging();
		EnableDragging(false);
		GetBody().SetLinearDamping(0.0f);
		 */
		Kill();
	}
}

void Nugget::Kill() {
	GetHealthManager().Kill();
	ShowEffect("star_collision");
	EmitBuff();
}

void Nugget::OnColliding(Body& body) {
	IW_CALLSTACK_SELF;
	
	if (!body.GetId().compare("star")) {
		// any non-star-sensor collision leads to selfdestruction
		Kill();
	}
		
	Body::OnColliding(body);
}

void Nugget::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;
	Body::OnUpdate(frame);
	
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

void Nugget::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;
	if (m_pxParticles) {
		m_pxParticles->Render(renderer, frame);
	}
	Body::OnRender(renderer, frame);
}

Nugget2::Nugget2(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef) : Nugget(id, bodydef, fixturedef, texturedef) {
}

const char* Nugget2::GetTypeName() {
	return Nugget2::TypeName();
}

const char* Nugget2::TypeName() {
	static const char* type = "nugget2";
	return type;
}

int Nugget2::GetDustAmount() {
	return GAME_POINTS_NUGGET2;
}

Nugget3::Nugget3(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef) : Nugget(id, bodydef, fixturedef, texturedef) {
}

const char* Nugget3::GetTypeName() {
	return Nugget3::TypeName();
}

const char* Nugget3::TypeName() {
	static const char* type = "nugget3";
	return type;
}

int Nugget3::GetDustAmount() {
	return GAME_POINTS_NUGGET3;
}

Nugget4::Nugget4(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef) : Nugget(id, bodydef, fixturedef, texturedef) {
}

const char* Nugget4::GetTypeName() {
	return Nugget4::TypeName();
}

const char* Nugget4::TypeName() {
	static const char* type = "nugget4";
	return type;
}

int Nugget4::GetDustAmount() {
	return GAME_POINTS_NUGGET4;
}
