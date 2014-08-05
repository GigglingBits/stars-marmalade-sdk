#include "Buff.h"
#include "Debug.h"

Buff::Buff(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef)
: Body(id, bodydef, fixturedef, texturedef) {
	SetRenderingLayer(Renderer::eRenderingLayerGameBackground);
	SetGravityScale(0.0f);
}

bool Buff::BeginDragging(const CIwFVec2& target, float dragforce) {
	ShowEffect("buff_touch");

	BuffArgs args;
	args.pos = GetCenter();
	Collected.Invoke(*this, args);
	
	GetHealthManager().Kill();
	return true;
}

void Buff::SetBuffAppearance(const std::string& skin, const std::string& animation) {
	IW_CALLSTACK_SELF;
	
	Texture* texture = GetTexture();
	IwAssert(MYAPP, texture);
	if (!texture) {
		return;
	}
	IwAssert(MYAPP, texture->IsSkeleton());
	if (!texture->IsSkeleton()) {
		return;
	}
	
	if (SpineAnimation* skeleton = texture->GetSkeleton()) {
		skeleton->SetSkin(skin);
		skeleton->SetAnimation(animation);
	}
}
