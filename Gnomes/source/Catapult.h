#ifndef __CATAPULT_H__
#define __CATAPULT_H__

#include "CompositeBody.h"
#include "Timer.h"

class Catapult : public CompositeBody {
public:
	Catapult(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);

	virtual const char* GetTypeName();
	static const char* TypeName();

protected:
	virtual void OnColliding(Body& thisbody, Body& otherbody);
};

#endif
