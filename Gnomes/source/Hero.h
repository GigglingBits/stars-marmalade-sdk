#ifndef __HERO_H__
#define __HERO_H__

#include "CompositeBody.h"

class Hero : public CompositeBody {
public:
	class StateBase {
	protected:
		Hero& m_rxContext;

	public:
		StateBase(Hero& context) : m_rxContext(context) {};
		virtual ~StateBase() {};

		virtual void Initialize() {};

		virtual void Stay() {};
		virtual void Jump(const CIwFVec2& impulse) {};
		virtual void Attack() {};
		virtual void Collide(Body& body) {};

		virtual void Update(uint16 timestep) {};
	};

protected:
	class IdleState;
	class JumpingState;
	class SmashingState;

private:
	StateBase* m_pxCurrentState;

public:
	Hero(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);
	virtual ~Hero();

	virtual const char* GetTypeName();
	static const char* TypeName();

	virtual void OnUpdate(const FrameData& frame);
	virtual void OnColliding(Body& thisbody, Body& otherbody);

	void SetState(StateBase* newstate);
	StateBase& GetCurrentState();

	void SetTextureFrame(std::string id);

	void Jump(const CIwFVec2& impulse); 
	void Attack();
	void Sit();
};

#endif
