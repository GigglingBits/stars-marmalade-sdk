#include "HudBuffButton.h"
#include "FactoryManager.h"
#include "UserSettings.h"
#include "Configuration.h"
#include "Main.h"

#include <sstream>

#include "Debug.h"
#include "IwDebug.h"

HudBuffButton::HudBuffButton(GameFoundation::BuffType bt) :
Button(eButtonCommandIdNone, s3eKeyFirst, bt), m_bIsUsed(false) {
	SetTexture(FactoryManager::GetTextureFactory().Create("buff"));
}

HudBuffButton::~HudBuffButton() {
	
}

bool HudBuffButton::CanUnload() {
	return m_bIsUsed;
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
	
	GameFoundation::BuffType bt = GetBuffType();
	BuffTrigger.Invoke(*this, bt);
	
	m_bIsUsed = true;
}
