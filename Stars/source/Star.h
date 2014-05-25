#ifndef __STAR_H__
#define __STAR_H__

#include <queue>
#include "Body.h"
#include "ParticleSystem.h"

class Star : public Body {
public:
	class MotionStateBase {
	protected:
		Star& m_rxContext;
		
	public:
		MotionStateBase(Star& context) : m_rxContext(context) {};
		virtual ~MotionStateBase() {};
		
		virtual void Initialize() {};
		virtual void FollowPath() {};
		
		virtual void Collide(Body& body) {};
		virtual void Update(uint16 timestep) {};
	};
	
	class AttackStateBase {
	protected:
		Star& m_rxContext;
		
	public:
		AttackStateBase(Star& context) : m_rxContext(context) {};
		virtual ~AttackStateBase() {};
		
		virtual void Initialize() {};
		
		virtual void BeginBlock() {};
		virtual void EndBlock() {};
		virtual void BeginAttack() {};
		virtual void EndAttack() {};
		
		virtual void Update(uint16 timestep) {};
	};
	
protected:
	class RetractingState;
	class FollowState;
	class RecoverState;

	class PeacefulState;
	class BlockState;
	class AttackState;
	
private:
	MotionStateBase* m_pxMotionState;
	AttackStateBase* m_pxAttackState;
	
	std::string m_sMotionTextureFrame;
	std::string m_sAttackTextureFrame;

	ParticleSystem* m_pxParticles;
	
private:
	std::queue<CIwFVec2> m_xPath;
	float m_fPathSpeed;
	float m_fAnchorLine;
	bool m_bAllowFlip;
	
public:
	Star(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);
	virtual ~Star();

	virtual const char* GetTypeName();
	static const char* TypeName();

	virtual void OnUpdate(const FrameData& frame);
	virtual void OnColliding(Body& thisbody, Body& otherbody);

	void SetAnchorLine(float xpos);

	void FollowPath(int samplecount, const CIwFVec2* samplepoints, float speed);
	
	void BeginBlock();
	void EndBlock();
	void BeginAttack();
	void EndAttack();
	
	bool IsFollowingPath();

private:
	void ClearPath();

	void AllowTextureFlip(bool allow);
	
	void SetMotionTextureFrame(const std::string& name);
	void SetAttackTextureFrame(const std::string& name);
	void ClearAttackTextureFrame();

	void SetTextureFrame(std::string id);
	
	void EnableParticles();
	void DisableParticles();
	
	void SetState(MotionStateBase* newstate);
	void SetState(AttackStateBase* newstate);
	MotionStateBase& GetMotionState();
	AttackStateBase& GetAttackState();

protected:
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	
public:
	enum DustEventType {
		eDustEventTypeCollectSingle,
		eDustEventTypeCollect,
		eDustEventTypeCommit,
		eDustEventTypeRollback,
	};
	struct DustEventArgs {
		DustEventType EventType;
		int amount;
		CIwFVec2 position;
	};
	MulticastEvent<Star, DustEventArgs> DustEvent;
};

#endif
