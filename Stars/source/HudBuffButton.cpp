#include "HudBuffButton.h"
#include "FactoryManager.h"
#include "UserSettings.h"
#include "Configuration.h"
#include "Main.h"

#include <sstream>

#include "Debug.h"
#include "IwDebug.h"

HudBuffButton::HudBuffButton(GameFoundation::BuffType bt) :
Button(eButtonCommandIdNone, s3eKeyFirst, bt), m_bIsUsed(false), m_iBuffLevel(0) {
	SetTexture(FactoryManager::GetTextureFactory().Create("buff"));
}

HudBuffButton::~HudBuffButton() {
}

bool HudBuffButton::CanUnload() {
	return m_bIsUsed;
}

void HudBuffButton::IncrementLevel() {
	m_iBuffLevel++;
	std::ostringstream oss;
	oss << m_iBuffLevel;
	m_sBuffLevel = oss.str();
}

GameFoundation::BuffType HudBuffButton::GetBuffType() {
	return (GameFoundation::BuffType)GetUserData();
}

std::string HudBuffButton::GetSkinName(GameFoundation::BuffType bt) {
	switch (bt) {
		case GameFoundation::eBuffTypeMagnet:
			return "magnet";
		case GameFoundation::eBuffTypeShield:
			return "shield";
		case GameFoundation::eBuffTypeShoot:
			return "shoot";
		default:
			IwAssertMsg(MYAPP, false, ("Buff not known: %i. It will be ignored.", bt));
	}
	return "";
}

void HudBuffButton::OnTextureLoaded(Texture& texture) {
	IW_CALLSTACK_SELF;
	
	IwAssert(MYAPP, texture.IsSkeleton());
	if (!texture.IsSkeleton()) {
		return;
	}
	
	if (SpineAnimation* skeleton = texture.GetSkeleton()) {
		skeleton->SetSkin(GetSkinName(GetBuffType()));
		skeleton->SetAnimation("button");
	}
	
	Button::OnTextureLoaded(texture);
}

void HudBuffButton::OnStateChanged(bool enabled, bool pressed) {
	if (!enabled || !pressed) {
		return;
	}
	
	BuffTriggerArgs args;
	args.bufftype = GetBuffType();
	args.bufflevel = m_iBuffLevel;
	BuffTrigger.Invoke(*this, args);
	
	m_bIsUsed = true;
}

void HudBuffButton::OnRender(Renderer& renderer, const FrameData& frame) {
	Button::OnRender(renderer, frame);
	
	if (!m_sBuffLevel.empty()) {
		renderer.DrawText(m_sBuffLevel, GetPosition());
	}
}
