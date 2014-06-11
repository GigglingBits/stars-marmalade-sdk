#include "BuffShoot.h"
#include "Debug.h"

BuffShoot::BuffShoot(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef)
: Buff(id, bodydef, fixturedef, texturedef) {
	SetBuffAppearance("shoot", "buff");
}

const char* BuffShoot::GetTypeName() {
	return BuffShoot::TypeName();
}

const char* BuffShoot::TypeName() {
	static const char* type = "buff_shoot";
	return type;
}
