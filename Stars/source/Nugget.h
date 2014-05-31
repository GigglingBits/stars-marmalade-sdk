#ifndef __NUGGET_H__
#define __NUGGET_H__

#include "Body.h"
#include "ParticleSystem.h"

class Nugget : public Body {
private:
	ParticleSystem* m_pxParticles;
	
public:
	Nugget(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);
	
	virtual const char* GetTypeName();
	static const char* TypeName();
	
	virtual int GetDustAmount();
	
protected:
	virtual void OnColliding(Body& thisbody, Body& otherbody);
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};


class Nugget2 : public Nugget {
public:
	Nugget2(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);

	virtual const char* GetTypeName();
	static const char* TypeName();

	virtual int GetDustAmount();
};

class Nugget3 : public Nugget {
public:
	Nugget3(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);

	virtual const char* GetTypeName();
	static const char* TypeName();

	virtual int GetDustAmount();
};

class Nugget4 : public Nugget {
public:
	Nugget4(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);

	virtual const char* GetTypeName();
	static const char* TypeName();

	virtual int GetDustAmount();
};

#endif
