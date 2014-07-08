#include "BirdRipper.h"
#include "Debug.h"

#include "SoundEngine.h"
#include "Main.h"

BirdRipper::BirdRipper(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef)
: Bird(id, bodydef, fixturedef, texturedef) {
	IW_CALLSTACK_SELF;
	
	SetTextureFrame("normal");
	
	if (SpineAnimation* sp = GetTextureSkeleton()) {
		sp->CustomEvent.AddListener(this, &BirdRipper::AnimationEventHandler);
	} else {
		IwAssertMsg(MYAPP, false, ("Cannot get texture animation for body of type '%s'", GetTypeName()));
	}
}

BirdRipper::~BirdRipper() {
	IW_CALLSTACK_SELF;
	
	if (SpineAnimation* sp = GetTextureSkeleton()) {
		sp->CustomEvent.RemoveListener(this, &BirdRipper::AnimationEventHandler);
	} else {
		IwAssertMsg(MYAPP, false, ("Cannot get texture animation for body of type '%s'", GetTypeName()));
	}
}

const char* BirdRipper::GetTypeName() {
	return BirdRipper::TypeName();
}

const char* BirdRipper::TypeName() {
	static const char* type = "birdripper";
	return type;
}

void BirdRipper::SetScreechCollision(bool enable) {
	IW_CALLSTACK_SELF;
	if (Body* child = GetChild("claws")) {
		child->EnableCollisions(enable);
	} else {
		IwAssertMsg(MYAPP, false, ("Could not set the screech collisions in body of type '%s'", GetTypeName()));
	}
}

void BirdRipper::KnockOut() {
	SetScreechCollision(false);
	Bird::KnockOut();
}

void BirdRipper::AnimationEventHandler(const SpineAnimation& sender, const SpineAnimation::EventArgs& args) {
	IW_CALLSTACK_SELF;
	
	if (!args.type.compare("custom")) {
		if (!args.string_param.compare("enable claws")) {
			SetScreechCollision(true);
		} else if (!args.string_param.compare("disable claws")) {
			SetScreechCollision(false);
		} else {
			IwAssertMsg(MYAPP, false, ("Recevied a custom event from the animation object, but did not understand the parameters '%s', '%f', '%i'.", args.string_param.c_str(), args.float_param, args.int_param));
		}
	}
}
