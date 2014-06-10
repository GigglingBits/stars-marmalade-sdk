#ifndef __BUFFMAGNET_H__
#define __BUFFMAGNET_H__

#include "Buff.h"

class BuffMagnet : public Buff {
public:
	BuffMagnet(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);

	virtual const char* GetTypeName();
	static const char* TypeName();
};

#endif
