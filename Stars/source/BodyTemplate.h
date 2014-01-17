#ifndef __BODYTEMPLATE_H__
#define __BODYTEMPLATE_H__

#include <map>
#include <string>
#include "IwGeom.h"
#include "Box2d.h"
#include "BodyJoint.h"

class BodyTemplate {
public:
	struct JointElement {
		std::string JointName;
		std::string ChildA;
		std::string PortA;
		std::string ChildB;
		std::string PortB;
		BodyJoint::eJointType JointType;
	};
	
	typedef std::map<std::string, CIwFVec2> PortMap;
	typedef std::map<std::string, std::string> ChildMap;
	typedef std::map<std::string, JointElement> JointMap;

private:
	b2BodyDef m_xDef;
	std::string m_sId;
	std::string m_sType;
	std::string m_sShapeId;
	std::string m_sFixtureId;
	std::string m_sTextureId;

	bool m_bDragEnabled;
	bool m_bRevolveEnabled;
	bool m_bCollideEnabled;

	ChildMap m_xChildren;
	PortMap m_xPorts;
	JointMap m_xJoints;

public:
	BodyTemplate();
	~BodyTemplate();

	void Set(const std::string& id, const std::string& type, const std::string& shape, const std::string& fixture, const std::string& texture);
	void AddChild(const std::string& childid, const std::string& bodyname, const std::string& port, const std::string& jointtype);
	void AddPort(const std::string& portid, const CIwFVec2& pos);
	void AddJoint(const std::string& jointid, const std::string& childa, const std::string& porta, const std::string& childb, const std::string& portb, const std::string& jointtype);

	b2BodyDef* CreatePhysicsBodyDef();

	const std::string& GetId();
	const std::string& GetType();
	const std::string& GetShapeId();
	const std::string& GetFixtureId();
	const std::string& GetTextureId();

	void SetEnableDrag(bool enable);
	bool IsDragEnabled();

	void SetEnableRevolve(bool enable);
	bool IsRevolveEnabled();
	
	void SetEnableCollide(bool enable);
	bool IsCollideEnabled();
	
	ChildMap& GetChildren();
	PortMap& GetPorts();
	JointMap& GetJoints();

	bool HasPort(const std::string& port) const;

private:
	bool CheckPort(const std::string& child, const std::string& port);
	BodyJoint::eJointType TranslateJointType(const std::string& jointtype);
};

#endif
