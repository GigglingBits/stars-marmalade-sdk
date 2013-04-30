#include "AppPanel.h"
#include "FactoryManager.h"

#include "Debug.h"

AppPanel::AppPanel(ButtonCommandId cmdid, s3eKey key, long userdata) :
ButtonPanel(cmdid, key, userdata),
m_xBtnQuit(eButtonCommandIdQuitLevel, s3eKeyEsc),
m_xBtnRestart(eButtonCommandIdRestartLevel, s3eKeyR),
m_xBtnReloadLocal(eButtonCommandIdReload, s3eKeyL, 0),
m_xBtnReload(eButtonCommandIdReload, s3eKeyD, 1) {
}

void AppPanel::Initialize() {
	m_xBtnReloadLocal.SetTexture(FactoryManager::GetTextureFactory().Create("button_reload_local"));
	m_xBtnReload.SetTexture(FactoryManager::GetTextureFactory().Create("button_reload"));
	m_xBtnQuit.SetTexture(FactoryManager::GetTextureFactory().Create("button_quit"));
	m_xBtnRestart.SetTexture(FactoryManager::GetTextureFactory().Create("button_restart"));

	AddButton(m_xBtnQuit);
	AddButton(m_xBtnReloadLocal);
	AddButton(m_xBtnReload);
	AddButton(m_xBtnRestart);
}
