#ifndef __BUFFSHIELD_H__
#define __BUFFSHIELD_H__

#include "Buff.h"

class BuffShield : public Buff {
public:
	BuffShield(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);

	virtual const char* GetTypeName();
	static const char* TypeName();
};

#endif
