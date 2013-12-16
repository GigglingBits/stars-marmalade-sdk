#include "Buff.h"
#include "Debug.h"
#include "FactoryManager.h"

Buff::Buff(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef) 
	: Body(id, bodydef, fixturedef, texturedef), m_pxParticles(NULL) {
}

const char* Buff::GetTypeName() {
	return Buff::TypeName();
}

const char* Buff::TypeName() {
	static const char* type = "buff";
	return type;
}

void Buff::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;
	Body::OnUpdate(frame);
	
	GetPosition();
	
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

void Buff::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;
	if (m_pxParticles) {
		m_pxParticles->Render(renderer, frame);
	}
	Body::OnRender(renderer, frame);
}
