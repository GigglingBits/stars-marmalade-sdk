#include "SocialPanel.h"
#include "FactoryManager.h"

SocialPanel::SocialPanel(ButtonCommandId cmdid, s3eKey key, long userdata) : 
ButtonPanel(cmdid, key, userdata),
m_xCredits(eButtonCommandIdCredits, s3eKeyFirst),
m_xAchievements(eButtonCommandIdAchievements, s3eKeyFirst) {
}

void SocialPanel::Initialize() {
	m_xCredits.SetTexture(FactoryManager::GetTextureFactory().Create("button_credits"));
	m_xAchievements.SetTexture(FactoryManager::GetTextureFactory().Create("button_achievements"));

	AddButton(m_xCredits);
	AddButton(m_xAchievements);
}
