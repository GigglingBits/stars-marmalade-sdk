#include "LevelAdsPanel.h"
#include "FactoryManager.h"

#include "Debug.h"

LevelAdsPanel::LevelAdsPanel() :
m_xBtnWatchAd(eButtonCommandIdQuitLevel, s3eKeyAbsGameD),
m_xBtnRemoveAds(eButtonCommandIdRestartLevel, s3eKeyR) {
//	SetAlpha(0x88);
}

void LevelAdsPanel::Initialize() {
	m_xBtnWatchAd.SetTexture(FactoryManager::GetTextureFactory().Create("button_ads_watch"));
//	AddButton(m_xBtnWatchAd);

	m_xBtnRemoveAds.SetTexture(FactoryManager::GetTextureFactory().Create("button_ads_remove"));
//	AddButton(m_xBtnRemoveAds);
}

