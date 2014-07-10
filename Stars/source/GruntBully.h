#ifndef __GRUNTBULLY_H__
#define __GRUNTBULLY_H__

#include "Grunt.h"

class GruntBully : public Grunt {
public:
	GruntBully(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);
	virtual ~GruntBully();
	
	virtual const char* GetTypeName();
	static const char* TypeName();
};

#endif
