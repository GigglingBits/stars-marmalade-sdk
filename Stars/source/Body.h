#ifndef __BODY_H__
#define __BODY_H__

#include <map>

#include "IwGeom.h"
#include "Event.h"
#include "Sprite.h"
#include "World.h"
#include "BodyShape.h"
#include "BodyHealth.h"

class Body : public Sprite {
private:
	World m_xWorld;

	b2Body* m_pxBody;
    b2Fixture* m_pxFixture;

	BodyShape m_xShape;

	bool m_bIsDynamic;

	bool m_bCanDrag;
	b2MouseJoint* m_pxDragHandle;

	BodyHealth m_xHealth;
	int m_iLastHealthValue;

	typedef std::map<std::string, CIwFVec2> PortMap;
	PortMap m_xPortMap;

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
	virtual CIwFVec2 GetPosition();
	CIwFVec2 GetCenter();

	void SetImpulse(const CIwFVec2& vector);
	virtual void SetSpeed(const CIwFVec2& vector);

	void EnableCollisions(bool enable);
	void EnableRotation(bool enable);

	bool HitTest(const CIwFVec2& position);

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
	virtual bool BeginDragging(const CIwFVec2& target);
	virtual void MoveDragging(const CIwFVec2& target);
	virtual void EndDragging();
	bool IsDragging();

	virtual float GetMass();

protected:
	b2Fixture& GetFixture();

	CIwFVec2 GetDragTarget();
	void SetDragForce(float maxforce);

	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnColliding(Body& thisbody, Body& otherbody);
	
public:
	struct EffectArgs {
		std::string id;
		CIwFVec2 pos;
	};
	Event<Body, EffectArgs> EffectRequested;
};

#endif
