#include "Catapult.h"
#include "Debug.h"
#include "SoundEngine.h"
#include "Hero.h"

Catapult::Catapult(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef) 
	: CompositeBody(id, bodydef, fixturedef, texturedef) {
}

const char* Catapult::GetTypeName() {
	return Catapult::TypeName();
}

const char* Catapult::TypeName() {
	static const char* type = "catapult";
	return type;
}

void Catapult::OnColliding(Body& thisbody, Body& otherbody) {
	if (otherbody.GetTypeName() == Hero::TypeName()) {
		otherbody.SetImpulse(CIwFVec2(0.0f, 10.0f));
	}
}
