#include <sstream>

#include "Body.h"
#include "Debug.h"
#include "IwDebug.h"
#include "Configuration.h"
#include "FactoryManager.h"

Body::Body(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef) 
	: Sprite(id, texturedef) {

	// construct physical body
	m_pxBody = m_xWorld.GetWorld().CreateBody(&bodydef);
	m_pxFixture = m_pxBody->CreateFixture(&fixturedef);
	m_bIsDynamic = m_pxBody->GetType() == b2_dynamicBody;

	// setting initial collision filtering data
	EnableCollisions(true);

	// register user data for collision detection
	m_pxBody->SetUserData(this);
	m_pxFixture->SetUserData(this);

	// origin port
	AddPort("", CIwFVec2::g_Zero);
		
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

void Body::SetGravityScale(float scale) {
	m_pxBody->SetGravityScale(scale);
}

float Body::GetGravityScale() {
	return m_pxBody->GetGravityScale();
}

void Body::SetSpeed(const CIwFVec2& vector) {
	m_pxBody->SetLinearVelocity(
		b2Vec2(vector.x, vector.y));
}

void Body::SetImpulse(const CIwFVec2& linear, float angular) {
	m_pxBody->ApplyLinearImpulse(
		b2Vec2(linear.x, linear.y),
		m_pxBody->GetWorldCenter(), true);
	m_pxBody->ApplyAngularImpulse(angular, true);
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

void Body::EmitBuff() {
	IW_CALLSTACK_SELF;
	EmitBuffArgs args;
	args.pos = GetPosition();
	EmitBuffRequested.Invoke(*this, args);
}

void Body::ShowEffect(const std::string& effect) {
	IW_CALLSTACK_SELF;
	EffectArgs args;
	args.id = effect;
	args.pos = GetPosition();
	EffectRequested.Invoke(*this, args);
}

void Body::Collide(Body& body) {
	OnColliding(body);
}

void Body::OnColliding(Body& body) {
	Colliding.Invoke(*this, body);
}

void Body::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;
	Sprite::OnUpdate(frame);

	// healh changed?
	int health = m_xHealth.GetHealthValue();
	if (m_iLastHealthValue != health) {
		m_iLastHealthValue = health;
		if (Texture* t = GetTexture()) {
			t->ReSelectFrame(health);
		}
	}

	UpdateShape();
}

void Body::UpdateShape() {
	// position might have changed?
	m_xShape.SetVertices(m_pxBody->GetTransform(), *m_pxFixture->GetShape());

	// push the shape of the body down into the base class
	Texture* t = GetTexture();
	VertexStreamWorld& v = GetShape();
	if (t && (t->IsImage() || t->IsSkeleton())) {
		v.SetVerts(
			m_xShape.GetBoundigBoxVerts(), 
			m_xShape.GetBoundigBoxVertCount());
	} else if (t && (t->IsPattern() || t->IsColour())) {
		v.SetVerts(
			m_xShape.GetShapeVerts(), 
			m_xShape.GetShapeVertCount());
	} else {
		IwError(("Unexpected shape"));
	}
}

void Body::OnRender(Renderer& renderer, const FrameData& frame) {
	Sprite::OnRender(renderer, frame);

	// print health value
	if (Configuration::GetInstance().ShowStats) {
		std::ostringstream oss;
		oss << m_xHealth.GetHealthValue();
		renderer.DrawText(
			oss.str(),
			GetPosition(),
			Renderer::eFontTypeSmall,
			0xffff0000);
	}
	
	if (Configuration::GetInstance().ShowShapes) {
		VertexStreamWorld shape;
		/*
		if (m_xShape.GetBoundigBoxVertCount() > 0) {
			shape.SetVerts(m_xShape.GetBoundigBoxVerts(), m_xShape.GetBoundigBoxVertCount());
			renderer.DrawPolygon(
				shape.GetVerts(),
				shape.GetVertCount(),
				0xff00ff00,
				0x00000000);
		}
		 */
		if (m_xShape.GetShapeVertCount() > 0) {
			shape.SetVerts(m_xShape.GetShapeVerts(), m_xShape.GetShapeVertCount());
			renderer.DrawPolygon(
				shape.GetVerts(),
				shape.GetVertCount(),
				0xff0000ff,
				IsDragging() ? 0x6600aa00 : 0x00000000);
		}
	}
}

