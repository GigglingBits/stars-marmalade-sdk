#include <sstream>


#include "LevelMenuButton.h"
#include "FactoryManager.h"
#include "UserSettings.h"

LevelMenuButton::LevelMenuButton() :
ButtonEx(eButtonCommandIdOpenLevel, s3eKeyFirst, -1),
m_iAchievedStars(0),
m_bLevelAvailable(false),
m_bReentrant(false) {
}

void LevelMenuButton::SetTextures(const std::string& face, const std::string& stars) {
	SetTexture(FactoryManager::GetTextureFactory().Create("button_level"));
}

void LevelMenuButton::SetLevel(int level, bool available, bool open, int stars) {
	SetUserData(level);
	m_bLevelAvailable = available;
	m_bLevelOpen = open;
	m_iAchievedStars = stars;
	
	if (open) {
		std::ostringstream oss;
		oss << level;
		SetText(oss.str(), 0xffccfaff, Renderer::eFontTypeLarge);
	} else {
		SetText("");
	}

	SetBehavior(IsEnabled());
}

void LevelMenuButton::OnStateChanged(bool enabled, bool pressed) {
	ButtonEx::OnStateChanged(enabled, pressed);
	SetBehavior(enabled);
}

void LevelMenuButton::SetBehavior(bool enabled) {
	if (m_bReentrant) {
		// protecting against recursive SetEnabled(bool) calls
		return;
	}

	m_bReentrant = true;
	
	SetHideWhenDisabled(!m_bLevelAvailable);
	SetEnabled(enabled && m_bLevelOpen);

	m_bReentrant = false;
}
