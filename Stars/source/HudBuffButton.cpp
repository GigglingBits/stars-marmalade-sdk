#include "HudBuffButton.h"
#include "FactoryManager.h"
#include "UserSettings.h"
#include "Configuration.h"
#include "Main.h"

#include <sstream>

#include "Debug.h"
#include "IwDebug.h"

HudBuffButton::HudBuffButton(const std::string& skin, ButtonCommandId cmdid, s3eKey key) :
Button(cmdid, key, -1),
m_sSkin(skin),
m_iFillPercent(0) {
	SetHideWhenDisabled(false);
	UpdateBehavior();
}

void HudBuffButton::SetCount(int count) {
	int buffthreshold = Configuration::GetInstance().BuffThreshold;
	int fillpercent = count * 100 / buffthreshold;
	fillpercent = std::max<int>(fillpercent, 0);
	fillpercent = std::min<int>(fillpercent, 100);
	m_iFillPercent = fillpercent;

	UpdateBehavior();
}

void HudBuffButton::UpdateBehavior() {
	// locking
	SetEnabled(m_iFillPercent >= 100);
	
	// text
	if (m_iFillPercent > 0) {
		std::ostringstream oss;
		oss << m_iFillPercent << "%";
		SetText(oss.str(), GAME_COLOUR_FONT_MAIN, Renderer::eFontTypeSmall);
	} else {
		SetText("");
	}
}

void HudBuffButton::OnTextureLoaded(Texture& texture) {
	IW_CALLSTACK_SELF;
	
	IwAssert(MYAPP, texture.IsSkeleton());
	if (!texture.IsSkeleton()) {
		return;
	}
	
	if (SpineAnimation* skeleton = texture.GetSkeleton()) {
		skeleton->SetSkin(m_sSkin);
		skeleton->SetAnimation("button");
	}
	
	Button::OnTextureLoaded(texture);
}

void HudBuffButton::OnRender(Renderer& renderer, const FrameData& frame) {
	Button::OnRender(renderer, frame);
}
