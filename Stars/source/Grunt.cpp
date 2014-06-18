#include "Grunt.h"
#include "Debug.h"

#include "SoundEngine.h"

Grunt::Grunt(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef)
	: Enemy(id, bodydef, fixturedef, texturedef) {
	SetTextureFrame("normal");
}

const char* Grunt::GetTypeName() {
	return Grunt::TypeName();
}

const char* Grunt::TypeName() {
	static const char* type = "grunt";
	return type;
}

void Grunt::KnockOut() {
	Enemy::KnockOut();

	SetTextureFrame("hurt");

	SoundEngine::GetInstance().PlaySoundEffect("GruntCollision");
}