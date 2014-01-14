#ifndef __BUFF_H__
#define __BUFF_H__

#include "Body.h"

class Buff : public Body {
public:
	Buff(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);

	virtual const char* GetTypeName();
	static const char* TypeName();
};

#endif
