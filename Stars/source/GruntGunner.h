#ifndef __GRUNTGUNNER_H__
#define __GRUNTGUNNER_H__

#include "Grunt.h"

class GruntGunner : public Grunt {
public:
	GruntGunner(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);
	virtual ~GruntGunner();
	
	virtual const char* GetTypeName();
	static const char* TypeName();
};

#endif
