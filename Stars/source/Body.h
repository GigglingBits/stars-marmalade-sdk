#ifndef __BODY_H__
#define __BODY_H__

#include <map>

#include "IwGeom.h"
#include "MulticastEvent.h"
#include "Sprite.h"
#include "World.h"
#include "BodyShape.h"
#include "BodyHealth.h"

class Body : public Sprite {
public:
	struct BuffProbabilities {
		float magnet;
		float shield;
		float shoot;
		BuffProbabilities() : magnet(0.0f), shield(0.0f), shoot(0.0f) {
		}
	};
	
private:
	World m_xWorld;

	b2Body* m_pxBody;
    b2Fixture* m_pxFixture;

	BodyShape m_xShape;

	bool m_bIsDynamic;

	bool m_bCanDrag;
	bool m_bEndDragging;
	b2MouseJoint* m_pxDragHandle;

	BodyHealth m_xHealth;
	int m_iLastHealthValue;

	typedef std::map<std::string, CIwFVec2> PortMap;
	PortMap m_xPortMap;

	BuffProbabilities m_xBuffProbabilities;
	
public:
	Body(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);
	virtual ~Body();

	virtual const char* GetTypeName();
	static const char* TypeName();

	virtual bool CanDispose();

	void AddPort(const std::string& portid, const CIwFVec2& pos);
	CIwFVec2* GetPort(const std::string& portid);

	virtual void SetPosition(const CIwFVec2& position);
	virtual void SetPosition(const CIwFVec2& position, float angle);
	virtual CIwFVec2 GetPosition() const;
	CIwFVec2 GetCenter();
	
	void SetImpulse(const CIwFVec2& linear, float angular = 0.0f);
	virtual void SetSpeed(const CIwFVec2& vector);

	virtual void EnableCollisions(bool enable);
	void EnableRotation(bool enable);

	virtual void SetGravityScale(float scale);
	float GetGravityScale();

	bool HitTest(const CIwFVec2& position);

	void SetBuffProbabilities(const BuffProbabilities& probs);
	void EmitBuff();
	
	void ShowEffect(const std::string& effect);
	
private:
	void DestroyBody();
	void RemoveShape(b2Fixture* fixture);

	void UpdateShape();

public:
	void Collide(Body& body);
	b2Body& GetBody();

	BodyHealth& GetHealthManager();

	void EnableDragging(bool enable);
	bool CanDrag();
	virtual bool BeginDragging(const CIwFVec2& target, float dragforce = 0.0f);
	virtual void MoveDragging(const CIwFVec2& target);
	virtual void EndDragging();
	bool IsDragging();

	virtual float GetMass();

protected:
	SpineAnimation* GetTextureSkeleton();
	
	b2Fixture& GetFixture();

	CIwFVec2 GetDragTarget();
	void SetDragForce(float maxforce);
	
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnColliding(Body& body);
	
private:
	void AnimationEventHandler(const SpineAnimation& sender, const SpineAnimation::EventArgs& args);

public:
	struct EmitBuffArgs {
		CIwFVec2 pos;
		BuffProbabilities probs;
	};
	MulticastEvent<Body, EmitBuffArgs> EmitBuffRequested;

	struct EffectArgs {
		std::string id;
		CIwFVec2 pos;
	};
	MulticastEvent<Body, EffectArgs> EffectRequested;
	
	MulticastEvent<Body, Body> Colliding;
	
	enum DustEventType {
		eDustEventTypeBegin,
		eDustEventTypeAdd,
		eDustEventTypeCommit,
		eDustEventTypeRollback,
	};
	struct DustEventArgs {
		DustEventType eventtype;
		int amount;
		CIwFVec2 position;
	};
	MulticastEvent<Body, DustEventArgs> DustEvent;
};

#endif
