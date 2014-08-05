#ifndef __BUFF_H__
#define __BUFF_H__

#include "Body.h"

class Buff : public Body {
public:
	Buff(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);

	virtual bool BeginDragging(const CIwFVec2& target, float dragforce = 0.0f);
	
protected:
	void SetBuffAppearance(const std::string& skin, const std::string& animation);
	
public:
	struct BuffArgs {
		CIwFVec2 pos;
	};
	MulticastEvent<Buff, BuffArgs> Collected;
};

#endif
