#ifndef __BUFF_H__
#define __BUFF_H__

#include "Body.h"

class Buff : public Body {
public:
	Buff(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);

	virtual const char* GetTypeName();
	static const char* TypeName();
	
	virtual bool BeginDragging(const CIwFVec2& target);
	
public:
	struct BuffArgs {
		CIwFVec2 pos;
	};
	MulticastEvent<Body, BuffArgs> EmitBuff;
};

#endif
