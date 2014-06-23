#include "BodyJoint.h"
#include "Body.h"
#include "Debug.h"

BodyJoint::BodyJoint() 
	: m_pxBodyA(NULL), m_pxBodyB(NULL), m_pxJoint(NULL), m_eType(eJointTypeUndefined) {
}

BodyJoint::~BodyJoint() {
	Separate();
}

void BodyJoint::SetId(const std::string& id) {
	m_sId = id;
}

const std::string& BodyJoint::GetId() {
	return m_sId;
}

void BodyJoint::Separate() {
	if (m_pxJoint) {
		if (b2Body* body = m_pxJoint->GetBodyA() ? m_pxJoint->GetBodyA() : m_pxJoint->GetBodyB()) {
			body->GetWorld()->DestroyJoint(m_pxJoint);
		} else {
			IwAssertMsg(MYAPP, false, ("Could not delete joint with id '%s'. No bodies seem to be attached.", m_sId.c_str()));
		}
		m_pxBodyA = NULL;
		m_pxBodyB = NULL;
		m_pxJoint = NULL;
		m_eType = eJointTypeUndefined;
	}
}

void BodyJoint::Join(Body& bodya, const CIwFVec2& porta, Body& bodyb, const CIwFVec2& portb, BodyJoint::eJointType jointtype) {
	IW_CALLSTACK_SELF;

	if (m_pxJoint) {
		Separate();
	}
	IwAssertMsg(MYAPP, !m_pxJoint && m_eType == eJointTypeUndefined, ("Physics joint already created for joint id '%s'.", m_sId.c_str()));
	m_pxBodyA = &bodya;
	m_pxBodyB = &bodyb;
	m_eType = jointtype;

	b2JointDef* jointdef = NULL;
	switch (jointtype) {
		case eJointTypeString: {
			b2RopeJointDef* def = new b2RopeJointDef(); 
			def->localAnchorA = b2Vec2(porta.x, porta.y);
			def->localAnchorB = b2Vec2(portb.x, portb.y);

			def->maxLength = 0.2f;

			jointdef = def;
			break; 
		}

		case eJointTypeRevolute: {
			b2RevoluteJointDef* def = new b2RevoluteJointDef(); 
			def->localAnchorA = b2Vec2(porta.x, porta.y);
			def->localAnchorB = b2Vec2(portb.x, portb.y);

			def->enableLimit = true;
			def->referenceAngle = 0.0f;
			def->lowerAngle = -0.5f * b2_pi;
			def->upperAngle = 0.5f * b2_pi;

			def->enableMotor = true;
			def->motorSpeed = 0.0f;
			def->maxMotorTorque = 0.005f;

			jointdef = def;
			break; 
		}

		case eJointTypeWeld: {
			b2WeldJointDef* def = new b2WeldJointDef(); 
			def->localAnchorA = b2Vec2(porta.x, porta.y);
			def->localAnchorB = b2Vec2(portb.x, portb.y);

			def->referenceAngle = 0.0f;

			def->frequencyHz = 0.0f;
			def->dampingRatio = 1.0f;

			jointdef = def;
			break; 
		}

		case eJointTypeUndefined: {
			break;
		}
	}

	if (!jointdef) {
		return;
	}

	jointdef->bodyA = &bodya.GetBody();
	jointdef->bodyB = &bodyb.GetBody();
	jointdef->collideConnected = false;

	m_pxJoint = bodya.GetBody().GetWorld()->CreateJoint(jointdef);	
	delete jointdef;
}

Body* BodyJoint::GetBodyA() {
	return m_pxBodyA;
}

Body* BodyJoint::GetBodyB() {
	return m_pxBodyB;
}

CIwFVec2 BodyJoint::GetOffset() {
	if (m_pxJoint) {
		// returns the ideal offset, not the actual 
		// offset (e.g. when joints are torn apart)
		b2Vec2 offset = 
			(m_pxJoint->GetAnchorA() - m_pxJoint->GetBodyA()->GetPosition()) 
			- (m_pxJoint->GetAnchorB() - m_pxJoint->GetBodyB()->GetPosition());
		return CIwFVec2(offset.x, offset.y);
	}
	return CIwFVec2::g_Zero;
}
