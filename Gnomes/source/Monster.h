#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "Body.h"
#include "Timer.h"

#define MONSTER_JUMP_PERIOD 4000 // milliseconds

class Monster : public Body {
private:
	Timer m_xBlinkTimer;
	Timer m_xJumpTimer;

public:
	Monster(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);

	virtual const char* GetTypeName();
	static const char* TypeName();

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);

private:
	void TimerElapsedEventHandler(const Timer& sender, const long& data);
};

#endif
