#ifndef __BODYJOINT_H__
#define __BODYJOINT_H__

#include "Box2D/Box2D.h"
#include "IwGeom.h"
#include <string>

class Body;

class BodyJoint {
public:
	enum eJointType {
		eJointTypeUndefined = 0,
		eJointTypeRevolute = 1,
		eJointTypeString = 2,
		eJointTypeWeld = 3
	};

private:
	eJointType m_eType;
	std::string m_sId;

	Body* m_pxBodyA;
	Body* m_pxBodyB;
	b2Joint* m_pxJoint;

public:
	BodyJoint();
	~BodyJoint();

	void SetId(const std::string& id);
	const std::string& GetId();
	
	void Join(Body& bodya, const CIwFVec2& porta, Body& bodyb, const CIwFVec2& portb, eJointType jointtype);
	void Separate();

	Body* GetBodyA();
	Body* GetBodyB();
	CIwFVec2 GetOffset();
};

#endif
