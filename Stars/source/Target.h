#ifndef __TARGET_H__
#define __TARGET_H__

#include "Body.h"
#include "CircularRays.h"

class Target : public Body {
private:
	CircularRays m_xRays;

public:
	Target(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);

	virtual const char* GetTypeName();
	static const char* TypeName();

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
