#include "LevelPausedPanel.h"
#include "FactoryManager.h"
#include "Configuration.h"

#include "Debug.h"

LevelPausedPanel::LevelPausedPanel(ButtonCommandId cmdid, s3eKey key, long userdata) :
ButtonPanel(cmdid, key, userdata),
m_xBtnMenu(eButtonCommandIdQuitLevel, s3eKeyAbsGameD),
m_xBtnRetry(eButtonCommandIdRestartLevel, s3eKeyR),
m_xBtnReloadLocal(eButtonCommandIdReload, s3eKeyL, 0),
m_xBtnReload(eButtonCommandIdReload, s3eKeyD, 1) {
	SetAlpha(0x88);
}

void LevelPausedPanel::Initialize() {
	m_xBtnMenu.SetTexture(FactoryManager::GetTextureFactory().Create("button_pause_menu"));
	AddButton(m_xBtnMenu);

	m_xBtnRetry.SetTexture(FactoryManager::GetTextureFactory().Create("button_pause_retry"));
	AddButton(m_xBtnRetry);

	
	if (Configuration::GetInstance().UnlockAll) {
		m_xBtnReloadLocal.SetTexture(FactoryManager::GetTextureFactory().Create("button_pause_reload_local"));
		AddButton(m_xBtnReloadLocal);

		m_xBtnReload.SetTexture(FactoryManager::GetTextureFactory().Create("button_pause_reload"));
		AddButton(m_xBtnReload);
	}
}
