#ifndef __BIRDSCREECHER_H__
#define __BIRDSCREECHER_H__

#include "Bird.h"

class BirdScreecher : public Bird {
public:
	BirdScreecher(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);

	virtual const char* GetTypeName();
	static const char* TypeName();
};

#endif
