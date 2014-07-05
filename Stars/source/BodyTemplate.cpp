#include "BodyTemplate.h"
#include "FactoryManager.h"
#include "IwDebug.h"

BodyTemplate::BodyTemplate() {
	m_xDef.allowSleep = true;
	m_xDef.bullet = false;

	m_bDragEnabled = false;
	m_bRevolveEnabled = true;
	m_bCollideEnabled = true;
}

BodyTemplate::~BodyTemplate() {
}

b2BodyDef* BodyTemplate::CreatePhysicsBodyDef() {
	return new b2BodyDef(m_xDef);
}

void BodyTemplate::Set(const std::string& id, const std::string& type, const std::string& shape, const std::string& fixture, const std::string& texture) {
	m_sId = id;

	if (!type.compare("dynamic") || !type.compare("star") || !type.compare("nugget") || !type.compare("nugget2") || !type.compare("nugget3") || !type.compare("nugget4") || !type.compare("enemy") || !type.compare("bird") || !type.compare("bird_ripper") || !type.compare("bird_screecher") || !type.compare("grunt")) {
		m_xDef.type = b2_dynamicBody;
	} else if (!type.compare("kinematic") || !type.compare("buff_magnet")  || !type.compare("buff_shield")  || !type.compare("buff_shoot") || !type.compare("splashtext")) {
		m_xDef.type = b2_kinematicBody;
	} else { 
		m_xDef.type = b2_staticBody;
	}

	m_sType = type;
	m_sShapeId = shape;
	m_sFixtureId = fixture;
	m_sTextureId = texture;
}

void BodyTemplate::AddChild(const std::string& childid, const std::string& bodyname, const std::string& port, const std::string& jointtype) {
	IwAssertMsg(MYAPP, m_xChildren.find(childid) == m_xChildren.end(), ("Body '%s' contains more than one child '%s'. The body template is invalid.", m_sType.c_str(), childid.c_str()));
	IwAssertMsg(MYAPP, m_xPorts.find(port) != m_xPorts.end(), ("Cannot add child to body '%s' because port '%s' is missing. The body template is invalid.", m_sType.c_str(), port.c_str()));

	m_xChildren[childid] = bodyname;

	if (!jointtype.empty()) {
		AddJoint("__" + childid, "" /*this*/, port, childid, "" /*origin*/, jointtype);
	}
}

void BodyTemplate::AddPort(const std::string& portid, const CIwFVec2& pos) {
	IwAssertMsg(MYAPP, m_xPorts.find(portid) == m_xPorts.end(), ("Body '%s' contains more than one port '%s'. The body template is invalid.", m_sType.c_str(), portid.c_str()));
	m_xPorts[portid] = pos;
}

bool BodyTemplate::HasPort(const std::string& port) const {
	return m_xPorts.find(port) != m_xPorts.end();
}

bool BodyTemplate::CheckPort(const std::string& child, const std::string& port) {
	if (child.empty()) {
		return true;
	} else {
		ChildMap::iterator itc = m_xChildren.find(child);
		if (itc != m_xChildren.end()) {
			if (port.empty() || FactoryManager::GetBodyFactory().GetConfig(itc->second).HasPort(port) ) {
				return true;
			} else {
				IwAssertMsg(MYAPP, false, ("Body '%s' contains a joint on child '%s', on unknown port '%s'. The body template is invalid.", m_sId.c_str(), child.c_str(), port.c_str()));
				return false;
			}
		} else {
			IwAssertMsg(MYAPP, false, ("Body '%s' contains a joint on unknown child '%s'. The body template is invalid.", m_sId.c_str(), child.c_str()));
			return false;
		}
	}
}

void BodyTemplate::AddJoint(const std::string& jointid, const std::string& childa, const std::string& porta, const std::string& childb, const std::string& portb, const std::string& jointtype) {
	if (!CheckPort(childa, porta)) return;
	if (!CheckPort(childb, portb)) return;
	BodyJoint::eJointType type = TranslateJointType(jointtype);
	if (BodyJoint::eJointTypeUndefined == type) {
		IwAssertMsg(MYAPP, false, ("Body '%s' contains a joint with type '%s'. The body template is invalid.", m_sType.c_str(), jointtype.c_str()));
		return;
	}

	JointElement& joint = m_xJoints[jointid];
	joint.JointName = jointid;
	joint.JointType = type;
	joint.ChildA = childa;
	joint.PortA = porta;
	joint.ChildB = childb;
	joint.PortB = portb;
}

const std::string& BodyTemplate::GetId() {
	return m_sId;
}

const std::string& BodyTemplate::GetType() {
	return m_sType;
}

const std::string& BodyTemplate::GetShapeId() {
	return m_sShapeId;
}

const std::string& BodyTemplate::GetFixtureId() {
	return m_sFixtureId;
}

const std::string& BodyTemplate::GetTextureId() {
	return m_sTextureId;
}

void BodyTemplate::SetEnableDrag(bool enable) {
	m_bDragEnabled = enable;
}

bool BodyTemplate::IsDragEnabled() {
	return m_bDragEnabled;
}

void BodyTemplate::SetEnableRevolve(bool enable) {
	m_bRevolveEnabled = enable;
}

bool BodyTemplate::IsRevolveEnabled() {
	return m_bRevolveEnabled;
}

void BodyTemplate::SetEnableCollide(bool enable) {
	m_bCollideEnabled = enable;
}

bool BodyTemplate::IsCollideEnabled() {
	return m_bCollideEnabled;
}

BodyTemplate::PortMap& BodyTemplate::GetPorts() {
	return m_xPorts;
}

BodyTemplate::ChildMap& BodyTemplate::GetChildren() {
	return m_xChildren;
}

BodyTemplate::JointMap& BodyTemplate::GetJoints() {
	return m_xJoints;
}

BodyJoint::eJointType BodyTemplate::TranslateJointType(const std::string& jointtype) {
	if (!jointtype.compare("weld")) {
		return BodyJoint::eJointTypeWeld;
	} else if (!jointtype.compare("revolute")) {
		return BodyJoint::eJointTypeRevolute;
	} else if (!jointtype.compare("string")) {
		return BodyJoint::eJointTypeString;
	} else if (jointtype.empty()) {
		return BodyJoint::eJointTypeUndefined;
	} else {
		return BodyJoint::eJointTypeUndefined;
	}
}
