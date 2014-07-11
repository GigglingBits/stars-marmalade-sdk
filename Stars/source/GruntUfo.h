#ifndef __GRUNTUFO_H__
#define __GRUNTUFO_H__

#include "Grunt.h"

class GruntUfo : public Grunt {
public:
	GruntUfo(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);
	virtual ~GruntUfo();
	
	virtual const char* GetTypeName();
	static const char* TypeName();
};

#endif
