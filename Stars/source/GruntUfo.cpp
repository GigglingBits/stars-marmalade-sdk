#include "GruntUfo.h"
#include "Debug.h"

#include "SoundEngine.h"
#include "Main.h"

GruntUfo::GruntUfo(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef)
	: Grunt(id, bodydef, fixturedef, texturedef) {
	IW_CALLSTACK_SELF;
		
	SetTextureFrame("normal");
}

GruntUfo::~GruntUfo() {
	IW_CALLSTACK_SELF;
}

const char* GruntUfo::GetTypeName() {
	return GruntUfo::TypeName();
}

const char* GruntUfo::TypeName() {
	static const char* type = "gruntufo";
	return type;
}
