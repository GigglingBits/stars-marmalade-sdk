#include "BirdRipper.h"
#include "Debug.h"

#include "SoundEngine.h"
#include "Main.h"

BirdRipper::BirdRipper(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef)
	: Bird(id, bodydef, fixturedef, texturedef) {
	SetTextureFrame("normal");
}

const char* BirdRipper::GetTypeName() {
	return BirdRipper::TypeName();
}

const char* BirdRipper::TypeName() {
	static const char* type = "birdripper";
	return type;
}

