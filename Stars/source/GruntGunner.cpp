#include "GruntGunner.h"
#include "Debug.h"

#include "SoundEngine.h"
#include "Main.h"

GruntGunner::GruntGunner(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef)
	: Grunt(id, bodydef, fixturedef, texturedef) {
	IW_CALLSTACK_SELF;
		
	SetTextureFrame("normal");
}

GruntGunner::~GruntGunner() {
	IW_CALLSTACK_SELF;
}

const char* GruntGunner::GetTypeName() {
	return GruntGunner::TypeName();
}

const char* GruntGunner::TypeName() {
	static const char* type = "gruntgunner";
	return type;
}
