#ifndef __GRUNT_H__
#define __GRUNT_H__

#include "Enemy.h"

class Grunt : public Enemy {
public:
	Grunt(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);

	virtual const char* GetTypeName();
	static const char* TypeName();
	
protected:
	virtual void KnockOut();
};

#endif
