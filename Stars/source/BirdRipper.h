#ifndef __BIRDRIPPER_H__
#define __BIRDRIPPER_H__

#include "Bird.h"

class BirdRipper : public Bird {
public:
	BirdRipper(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);
	virtual ~BirdRipper();
	
	virtual const char* GetTypeName();
	static const char* TypeName();
	
	virtual void KnockOut();

private:
	void SetScreechCollision(bool enable);
	
	void AnimationEventHandler(const SpineAnimation& sender, const SpineAnimation::EventArgs& args);
};

#endif
