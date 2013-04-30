#include "Monster.h"
#include "Hero.h"
#include "Debug.h"
#include "SoundEngine.h"

Monster::Monster(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef) 
	: Body(id, bodydef, fixturedef, texturedef) {
	EnableRotation(false);
	GetTexture().SelectFrame("idle");

	m_xJumpTimer.ElapsedEvent.AddListener<Monster>(this, &Monster::TimerElapsedEventHandler);
	m_xBlinkTimer.ElapsedEvent.AddListener<Monster>(this, &Monster::TimerElapsedEventHandler);

	m_xBlinkTimer.Start(3000, 7000, 0);
	m_xJumpTimer.Start(5000, 0);
}

const char* Monster::GetTypeName() {
	return Monster::TypeName();
}

const char* Monster::TypeName() {
	static const char* type = "monster";
	return type;
}

void Monster::TimerElapsedEventHandler(const Timer& sender, const long& data) {
	if (&sender == &m_xBlinkTimer) {
		GetTexture().SelectFrame("blink");
		m_xBlinkTimer.Start(3000, 7000, 0);

	} else if (&sender == &m_xJumpTimer) {
		if (data == 0) {
			GetTexture().SelectFrame("jump");
			m_xJumpTimer.Start(1000, 1);
			SoundEngine::PlaySoundEffect("NinjaOh");
		} else if (data == 1) {
			SetImpulse(CIwFVec2(0.0f, 2.0f));
			m_xJumpTimer.Start(5000, 0);
			SoundEngine::PlaySoundEffect("NinjaWaka");
		}
	}
}

void Monster::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xBlinkTimer.Update(frame.GetSimulatedDurationMs());
	m_xJumpTimer.Update(frame.GetSimulatedDurationMs());

	Body::OnUpdate(frame);
}

void Monster::OnRender(Renderer& renderer, const FrameData& frame) {
	Body::OnRender(renderer, frame);
}
