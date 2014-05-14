#include "TextureAnimated.h"
#include "Debug.h"

TextureAnimated::TextureAnimated(const TextureTemplate& texturedef) {
	Load(texturedef.GetSkeletonAnimation());
}

void TextureAnimated::Load(const std::string& skeleton) {
	m_xAnimation.Load(skeleton);
}

bool TextureAnimated::IsSkeleton() {
	return true;
}

BufferedAnimTexture* TextureAnimated::GetSkeleton() {
	return &m_xAnimation;
}

bool TextureAnimated::ContainsFrame(const std::string name) {
	IW_CALLSTACK_SELF;
	IwError(("TextureAnimated::ContainsFrame(const std::string name) not implemented"));
	return false;
}

bool TextureAnimated::SelectFrame(const std::string& id) {
	return m_xAnimation.SetAnimation(id);
}

bool TextureAnimated::ReSelectFrame(int health) {
	IW_CALLSTACK_SELF;
	IwError(("TextureAnimated::ReSelectFrame(int health) not implemented"));
	return true;
}

bool TextureAnimated::SelectFrame(const std::string& id, int health) {
	// ignore the health; it is not used in animations
	return SelectFrame(id);
}

void TextureAnimated::SetHorizontalFlip(bool flip) {
	IW_CALLSTACK_SELF;
	IwError(("TextureAnimated::SelectFrame(const std::string& id, int health) not implemented"));
}

bool TextureAnimated::GetHorizontalFlip() {
	IW_CALLSTACK_SELF;
	IwError(("TextureAnimated::GetHorizontalFlip() not implemented"));
	return false;
}

void TextureAnimated::Update(uint16 timestep) {
	m_xAnimation.Update(timestep);
}
