#include "SocialPanel.h"
#include "FactoryManager.h"

SocialPanel::SocialPanel(ButtonCommandId cmdid, s3eKey key, long userdata) : 
ButtonPanel(cmdid, key, userdata),
m_xAchievements(eButtonCommandIdAchievements, s3eKeyFirst),
m_xLeaderboards(eButtonCommandIdLeaderboards, s3eKeyFirst) {
}

void SocialPanel::Initialize() {
	m_xAchievements.SetTexture(FactoryManager::GetTextureFactory().Create("button_achievements"));
	m_xLeaderboards.SetTexture(FactoryManager::GetTextureFactory().Create("button_leaderboards"));

	AddButton(m_xAchievements);
	AddButton(m_xLeaderboards);
}
