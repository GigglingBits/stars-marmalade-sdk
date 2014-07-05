#include "BirdScreecher.h"
#include "Debug.h"

#include "SoundEngine.h"
#include "Main.h"

BirdScreecher::BirdScreecher(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef)
	: Bird(id, bodydef, fixturedef, texturedef) {
	SetTextureFrame("normal");
}

const char* BirdScreecher::GetTypeName() {
	return BirdScreecher::TypeName();
}

const char* BirdScreecher::TypeName() {
	static const char* type = "birdscreecher";
	return type;
}

