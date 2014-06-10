#include "BuffShield.h"
#include "Debug.h"

BuffShield::BuffShield(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef)
: Buff(id, bodydef, fixturedef, texturedef) {
}

const char* BuffShield::GetTypeName() {
	return BuffShield::TypeName();
}

const char* BuffShield::TypeName() {
	static const char* type = "buff_shield";
	return type;
}
