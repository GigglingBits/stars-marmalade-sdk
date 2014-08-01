#ifndef __STAR_H__
#define __STAR_H__

#include <queue>
#include "CompositeBody.h"
#include "ParticleSystem.h"
#include "PathTracker.h"

class Star : public CompositeBody {
public:
	class MotionStateBase {
	protected:
		Star& m_rxContext;
		
	public:
		MotionStateBase(Star& context) : m_rxContext(context) {};
		virtual ~MotionStateBase() {};
		
		virtual void Initialize() {};
		virtual void FollowPath() {};

		virtual void Passify() {};
		virtual void Collide(Body& body) {};
		virtual void Update(uint16 timestep) {};
	};
		
protected:
	class PassiveState;
	class RetractingState;
	class FollowState;
	class FallingState;
	
private:
	MotionStateBase* m_pxMotionState;
	
	PathTracker m_xPath;
	float m_fPathSpeed;
	float m_fAnchorLine;
	bool m_bAutoOrient;

	uint32 m_uiShieldDuration;
	
	ParticleSystem* m_pxParticles;

public:
	Star(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);
	virtual ~Star();

	virtual const char* GetTypeName();
	static const char* TypeName();

	virtual void OnUpdate(const FrameData& frame);
	virtual void OnColliding(Body& body);

	void SetAnchorLine(float xpos);
	void FollowPath(const std::vector<CIwFVec2>& path);
	
	void Passify();
	
	void BeginShield(uint32 duration);
	void EndShield();
	
	bool IsFollowingPath();

private:
	void AutoOrientTexture(bool allow);
		
	void EnableParticles();
	void DisableParticles();
	
	void SetState(MotionStateBase* newstate);
	MotionStateBase& GetMotionState();

protected:
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	
public:
	MulticastEvent<Star, int> Killed;
};

#endif
