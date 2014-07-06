#include "BirdScreecher.h"
#include "Debug.h"

#include "SoundEngine.h"
#include "Main.h"

BirdScreecher::BirdScreecher(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef)
	: Bird(id, bodydef, fixturedef, texturedef) {
	IW_CALLSTACK_SELF;
		
	SetTextureFrame("normal");
		
	if (SpineAnimation* sp = GetTextureSkeleton()) {
		sp->CustomEvent.AddListener(this, &BirdScreecher::AnimationEventHandler);
	} else {
		IwAssertMsg(MYAPP, false, ("Cannot get texture animation for body of type '%s'", GetTypeName()));
	}
}

BirdScreecher::~BirdScreecher() {
	IW_CALLSTACK_SELF;

	if (SpineAnimation* sp = GetTextureSkeleton()) {
		sp->CustomEvent.RemoveListener(this, &BirdScreecher::AnimationEventHandler);
	} else {
		IwAssertMsg(MYAPP, false, ("Cannot get texture animation for body of type '%s'", GetTypeName()));
	}
}

const char* BirdScreecher::GetTypeName() {
	return BirdScreecher::TypeName();
}

const char* BirdScreecher::TypeName() {
	static const char* type = "birdscreecher";
	return type;
}

void BirdScreecher::SetScreechCollision(bool enable) {
	IW_CALLSTACK_SELF;
	if (Body* child = GetChild("screech")) {
		child->EnableCollisions(enable);
	} else {
		IwAssertMsg(MYAPP, false, ("Could not set the screech collisions in body of type '%s'", GetTypeName()));
	}
}

void BirdScreecher::KnockOut() {
	SetScreechCollision(false);
	Bird::KnockOut();
}

void BirdScreecher::AnimationEventHandler(const SpineAnimation& sender, const SpineAnimation::EventArgs& args) {
	IW_CALLSTACK_SELF;

	if (!args.type.compare("custom")) {
		if (!args.string_param.compare("enable screech")) {
			SetScreechCollision(true);
		} else if (!args.string_param.compare("disable screech")) {
			SetScreechCollision(false);
		} else {
			IwAssertMsg(MYAPP, false, ("Recevied a custom event from the animation object, but did not understand the parameters '%s', '%f', '%i'.", args.string_param.c_str(), args.float_param, args.int_param));
		}
	}
}
