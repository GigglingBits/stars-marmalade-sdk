#ifndef __STAR_H__
#define __STAR_H__

#include <queue>
#include "Body.h"

class Star : public Body {
public:
	class StateBase {
	protected:
		Star& m_rxContext;

	public:
		StateBase(Star& context) : m_rxContext(context) {};
		virtual ~StateBase() {};

		virtual void Initialize() {};

		virtual void Stay() {};
		virtual void FollowPath() {};
		virtual void Jump(const CIwFVec2& impulse) {};
		virtual void Attack() {};
		virtual void Collide(Body& body) {};

		virtual void Update(uint16 timestep) {};
	};

protected:
	class IdleState;
	class FollowPathState;
	class JumpingState;
	class SmashingState;

private:
	StateBase* m_pxCurrentState;
	Texture* m_pxTouchTexture;

private:
	std::queue<CIwFVec2> m_xPath;
	float m_fAnchorLine;
	
public:
	Star(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);
	virtual ~Star();

	virtual const char* GetTypeName();
	static const char* TypeName();

	virtual void OnUpdate(const FrameData& frame);
	virtual void OnColliding(Body& thisbody, Body& otherbody);

	void SetState(StateBase* newstate);
	StateBase& GetCurrentState();

	void SetAnchorLine(float xpos);
	void SetTextureFrame(std::string id);

	void FollowPath(int samplecount, const CIwFVec2* samplepoints);
	void Jump(const CIwFVec2& impulse);
	void Attack();
	void Sit();
	
protected:
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
