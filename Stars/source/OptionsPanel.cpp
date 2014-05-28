#include "OptionsPanel.h"
#include "FactoryManager.h"

OptionsPanel::OptionsPanel(ButtonCommandId cmdid, s3eKey key, long userdata) : 
ButtonPanel(cmdid, key, userdata),
m_xReloadLocal(eButtonCommandIdReload, s3eKeyAbsGameB, 0),
m_xReload(eButtonCommandIdReload, s3eKeyAbsGameC, 1) {
}

OptionsPanel::~OptionsPanel() {
}

void OptionsPanel::Initialize() {
	m_xReloadLocal.SetTexture(FactoryManager::GetTextureFactory().Create("button_reload_local"));
	m_xReload.SetTexture(FactoryManager::GetTextureFactory().Create("button_reload"));

	AddButton(m_xReloadLocal);
	AddButton(m_xReload);
}

void OptionsPanel::OnUpdate(const FrameData& frame) {
	ButtonPanel::OnUpdate(frame);
}

