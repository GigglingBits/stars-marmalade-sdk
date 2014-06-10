#ifndef __BUFFSHOOT_H__
#define __BUFFSHOOT_H__

#include "Buff.h"

class BuffShoot : public Buff {
public:
	BuffShoot(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);

	virtual const char* GetTypeName();
	static const char* TypeName();
};

#endif
