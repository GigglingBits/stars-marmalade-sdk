#ifndef __COMPOSITEBODY_H__
#define __COMPOSITEBODY_H__

#include <map>
#include "Body.h"
#include "BodyJoint.h"

class CompositeBody : public Body {
private:
	Body* m_pxParentBody;

	typedef std::map<std::string, Body*> ChildList;
	ChildList m_xChildList;

	typedef std::map<std::string, BodyJoint> JointList;
	JointList m_xJointList;

	struct BodyNode {
		Body* parent;
		Body* child;
		CIwFVec2 offset;
	};
	typedef std::vector<BodyNode> BodyHierarchy;

public:
	CompositeBody(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);
	virtual ~CompositeBody();

	void AddChild(const std::string& childid, const std::string& bodyid);
	Body* GetChild(const std::string& childid);
	void RemoveChild(const std::string& childid);

	void AddJoint(const std::string& jointid, const std::string& childaa, const std::string& porta, const std::string& childb, const std::string& portb, BodyJoint::eJointType jointtype);
	void RemoveJoint(const std::string&jointid);

	virtual void SetPosition(const CIwFVec2& position, float angle);
	virtual void SetSpeed(const CIwFVec2& vector);

	virtual float GetMass();
	
	virtual void SetGravityScale(float scale);

private:
	void AddChild(Body* body);
	void DestroyChild(Body* body);

	void DestroyJoints();

	void AlignChildren();
	bool TryAlignChild(BodyJoint& joint, BodyHierarchy& bodies, bool force=false);
	bool TryAlignChildAddInfo(Body* parent, BodyJoint& joint, BodyHierarchy& bodies);

protected:	
	void DestroyChildren();

	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnChildColliding(Body& child, Body& body);
	
private:
	void ChildCollisionHandler(const Body& sender, const Body& body);
};

#endif
