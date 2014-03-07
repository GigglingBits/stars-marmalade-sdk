#ifndef __BIRD_H__
#define __BIRD_H__

#include "Enemy.h"

class Bird : public Enemy {
public:
	Bird(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);

	virtual const char* GetTypeName();
	static const char* TypeName();
	
protected:
	virtual void KnockOut();
};

#endif
