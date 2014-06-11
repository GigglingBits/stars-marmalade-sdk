#include "BuffMagnet.h"
#include "Debug.h"

BuffMagnet::BuffMagnet(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef)
: Buff(id, bodydef, fixturedef, texturedef) {
	SetBuffAppearance("magnet", "buff");
}

const char* BuffMagnet::GetTypeName() {
	return BuffMagnet::TypeName();
}

const char* BuffMagnet::TypeName() {
	static const char* type = "buff_magnet";
	return type;
}

