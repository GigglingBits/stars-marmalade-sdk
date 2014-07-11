#include "GruntBully.h"
#include "Debug.h"

#include "SoundEngine.h"
#include "Main.h"

GruntBully::GruntBully(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef)
	: Grunt(id, bodydef, fixturedef, texturedef) {
	IW_CALLSTACK_SELF;
		
	SetTextureFrame("normal");
}

GruntBully::~GruntBully() {
	IW_CALLSTACK_SELF;
}

const char* GruntBully::GetTypeName() {
	return GruntBully::TypeName();
}

const char* GruntBully::TypeName() {
	static const char* type = "gruntbully";
	return type;
}
