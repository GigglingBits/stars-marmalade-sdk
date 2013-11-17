#include <sstream>

#include "Body.h"
#include "Debug.h"
#include "IwDebug.h"
#include "Configuration.h"
#include "FactoryManager.h"

Body::Body(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef) 
	: Sprite(id, texturedef), m_pxGameFoundation(NULL) {

	// construct physical body
	m_pxBody = m_xWorld.GetWorld().CreateBody(&bodydef);
	m_pxFixture = m_pxBody->CreateFixture(&fixturedef);
	m_bIsDynamic = m_pxBody->GetType() == b2_dynamicBody;

	// setting initial collision filtering data
	EnableCollisions(true);

	// register user data for collision detection
	m_pxBody->SetUserData(this);
	m_pxFixture->SetUserData(this);

	// for texture tracking
	m_iLastHealthValue = m_xHealth.GetHealthValue();

	// show different texture
	m_bCanDrag = false;
	m_pxDragHandle = NULL;

	// set the shape for initial rendering
	UpdateShape();
}

Body::~Body() {
	if (IsDragging()) {
		EndDragging();
	}

	m_pxBody->DestroyFixture(m_pxFixture);
	m_xWorld.GetWorld().DestroyBody(m_pxBody);
}

const char* Body::GetTypeName() {
	return Body::TypeName();
}

const char* Body::TypeName() {
	static const char* type = "body";
	return type;
}

bool Body::CanDispose() {
	return m_xHealth.IsDead();
}

GameFoundation* Body::GetGameFoundation() {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, m_pxGameFoundation, ("Game foundation is queried but not known."));
	return m_pxGameFoundation;
}

void Body::SetGameFoundation(const GameFoundation& game) {
	m_pxGameFoundation = (GameFoundation*)&game;
}

void Body::AddPort(const std::string& portid, const CIwFVec2& pos) {
	m_xPortMap[portid] = pos;
}

CIwFVec2* Body::GetPort(const std::string& portid) {
	PortMap::iterator it = m_xPortMap.find(portid);
	if (it != m_xPortMap.end()) {
		return &it->second;
	}
	return NULL;
}

BodyHealth& Body::GetHealthManager() {
	return m_xHealth;
}

b2Body& Body::GetBody() {
	return *m_pxBody;
}

b2Fixture& Body::GetFixture() {
	return *m_pxFixture;
}

CIwFVec2 Body::GetPosition() {
	b2Vec2 pos = m_pxBody->GetPosition();
	return  CIwFVec2(pos.x, pos.y);
}

void Body::SetPosition(const CIwFVec2& position) {
	SetPosition(position, m_pxBody->GetAngle());
}

void Body::SetPosition(const CIwFVec2& position, float angle) {
	m_pxBody->SetTransform(b2Vec2(position.x, position.y), angle);
	UpdateShape();
}

CIwFVec2 Body::GetCenter() {
	b2Vec2 center = m_pxBody->GetPosition();

	b2MassData mass;
	m_pxBody->GetMassData(&mass);
	center += mass.center;

	return  CIwFVec2(center.x, center.y);
}

float Body::GetMass() {
	return m_pxBody->GetMass();
}

void Body::SetSpeed(const CIwFVec2& vector) {
	m_pxBody->SetLinearVelocity(
		b2Vec2(vector.x, vector.y));
}

void Body::SetImpulse(const CIwFVec2& vector) {
	m_pxBody->ApplyLinearImpulse(
		b2Vec2(vector.x, vector.y),
		m_pxBody->GetWorldCenter(), true);
}

void Body::EnableCollisions(bool enable) {
	b2Fixture* fixture = GetBody().GetFixtureList();
	while (fixture) {
		// disable all collisions
		b2Filter filter;
		filter.maskBits = enable ? 0x0001 : 0x0000;
		filter.categoryBits = enable ? 0x0001 : 0x0000;
		filter.groupIndex = enable ? 0x0001 : 0x0000;
		fixture->SetFilterData(filter);

		fixture = fixture->GetNext();
	}
}

void Body::EnableRotation(bool enable) {
	GetBody().SetFixedRotation(!enable);
}

void Body::EnableDragging(bool enable) {
	m_bCanDrag = enable;
}

bool Body::CanDrag() {
	return m_bCanDrag;
}

CIwFVec2 Body::GetDragTarget() {
	if (IsDragging()) {
		b2Vec2 target(m_pxDragHandle->GetTarget());
		return CIwFVec2(target.x, target.y);
	} else {
		return CIwFVec2::g_Zero;
	}
}

bool Body::IsDragging() {
	return m_pxDragHandle != NULL;
}

void Body::SetDragForce(float maxforce) {
	if (IsDragging()) {
		m_pxDragHandle->SetMaxForce(maxforce);
	}
}

bool Body::BeginDragging(const CIwFVec2& target) {
	if (!m_bCanDrag) {
		return false;
	}
	if (!HitTest(target)) {
		// don't allow dragging to be started outside the body
		return false;
	}
	if (m_pxBody->GetType() != b2_dynamicBody) {
		// static bodies and kinematic bodies don't 
		// follow the rules of physics; pinning is not supported
		return false;
	}
	if (IsDragging()) {
		// don't allow multiple drags on the same body
		return false;
	}

	b2MouseJointDef def;
	def.target = b2Vec2(target.x, target.y); // pin at actual touch position
	def.bodyA = &m_xWorld.GetAnchorBody();
	def.bodyB = m_pxBody;
	def.maxForce = 0.0f; //30.0f * m_pxBody->GetMass();
	def.dampingRatio = 1.0f;
	
	b2Joint* joint = m_pxBody->GetWorld()->CreateJoint(&def);
	m_pxDragHandle = dynamic_cast<b2MouseJoint*>(joint);
	IwAssertMsg(MYAPP, m_pxDragHandle, ("Failed to create a mouse joint. Some joint was created. But not a mouse joint...")); 

	return true;
}

void Body::MoveDragging(const CIwFVec2& target) {
	if (m_pxDragHandle) {
		m_pxDragHandle->SetTarget(b2Vec2(target.x, target.y));
	}
}

void Body::EndDragging() {
	if (m_pxDragHandle) {
		m_pxBody->GetWorld()->DestroyJoint(m_pxDragHandle);
		m_pxDragHandle = NULL;
	}
}

bool Body::HitTest(const CIwFVec2& position) {
	if (!m_pxFixture) {
		return false;
	}
	b2Shape* pShape = m_pxFixture->GetShape();
	return pShape->TestPoint(
		m_pxFixture->GetBody()->GetTransform(),
		b2Vec2(position.x, position.y));
}

void Body::ShowEffect(const std::string& effect) {
	IW_CALLSTACK_SELF;
	if (GameFoundation* game = GetGameFoundation()) {
		Sprite* smoke = FactoryManager::GetEffectFactory().Create(effect);
		smoke->SetPosition(GetPosition());
		game->Add(smoke);
	} else {
		IwAssertMsg(MYAPP, false, 
			("Tried to show effect %s for body %s. But the game foundation cannot be found!", effect.c_str(), GetId().c_str()));
	}
}

void Body::Collide(Body& body) {
	OnColliding(*this, body);
}

void Body::OnColliding(Body& thisbody, Body& otherbody) {
}

void Body::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;
	Sprite::OnUpdate(frame);

	// healh changed?
	int health = m_xHealth.GetHealthValue();
	if (m_iLastHealthValue != health) {
		m_iLastHealthValue = health;
		GetTexture().ReSelectFrame(health);
	}

	UpdateShape();
}

void Body::UpdateShape() {
	// position might have changed?
	m_xShape.SetVertices(m_pxBody->GetTransform(), *m_pxFixture->GetShape());

	// push the shape of the body down into the base class
	Texture& t = GetTexture();
	VertexStreamWorld& v = GetShape();
	if (t.IsImage()) {
		v.SetVerts(
			m_xShape.GetBoundigBoxVerts(), 
			m_xShape.GetBoundigBoxVertCount());
	} else if (t.IsPattern() || t.IsColour()) {
		v.SetVerts(
			m_xShape.GetShapeVerts(), 
			m_xShape.GetShapeVertCount());
	} else {
		IwError(("Unexpected shape"));
	}
}

void Body::OnRender(Renderer& renderer, const FrameData& frame) {
	Sprite::OnRender(renderer, frame);

	VertexStreamWorld& shape = GetShape();
	if (IsDragging() && Configuration::GetInstance().ShowStats && shape.GetVertCount() > 0) {
		// shade the shape if it is pinned
		renderer.DrawPolygon(
			shape.GetVerts(),
			shape.GetVertCount(),
			0x00000000,
			0x6600aa00);
	}

	// print health value
	std::ostringstream oss;
	oss << m_xHealth.GetHealthValue();
	renderer.DrawText(
		oss.str(),
		GetPosition(),
		Renderer::eFontTypeSmall, 
		0xffff0000);
}

