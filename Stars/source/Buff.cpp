#include "Buff.h"
#include "Debug.h"

Buff::Buff(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef)
: Body(id, bodydef, fixturedef, texturedef) {
	SetRenderingLayer(Renderer::eRenderingLayerGameBackground);
	SetGravityScale(0.0f);
}

const char* Buff::GetTypeName() {
	return Buff::TypeName();
}

const char* Buff::TypeName() {
	static const char* type = "buff";
	return type;
}

bool Buff::BeginDragging(const CIwFVec2& target) {
	GetHealthManager().Kill();
	ShowEffect("buff_touch");
	return false;
}
