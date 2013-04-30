#include "Target.h"
#include "Debug.h"

Target::Target(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef) 
	: Body(id, bodydef, fixturedef, texturedef) {
	EnableRotation(false);
	SetRederingLayer(Renderer::eRenderingLayerStaticGameObjects);
}

const char* Target::GetTypeName() {
	return Target::TypeName();
}

const char* Target::TypeName() {
	static const char* type = "target";
	return type;
}

void Target::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;
	b2Vec2 b2pos = GetBody().GetWorldCenter();
	CIwFVec2 pos(b2pos.x, b2pos.y);
	m_xRays.SetPosition(pos);
	m_xRays.Update(frame);
	Body::OnUpdate(frame);
}

void Target::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;
	m_xRays.Render(renderer, frame);
	Body::OnRender(renderer, frame);
}
