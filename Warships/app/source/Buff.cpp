#include "Buff.h"
#include "Debug.h"

Buff::Buff(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef) 
	: Body(id, bodydef, fixturedef, texturedef) {
}

const char* Buff::GetTypeName() {
	return Buff::TypeName();
}

const char* Buff::TypeName() {
	static const char* type = "buff";
	return type;
}

void Buff::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;
	b2Vec2 b2pos = GetBody().GetWorldCenter();
	CIwFVec2 pos(b2pos.x, b2pos.y);
	m_xRays.SetPosition(pos);
	m_xRays.Update(frame);
	Body::OnUpdate(frame);
}

void Buff::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;
	m_xRays.Render(renderer, frame);
	Body::OnRender(renderer, frame);
}
