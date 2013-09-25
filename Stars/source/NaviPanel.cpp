#include "NaviPanel.h"
#include "FactoryManager.h"

NaviPanel::NaviPanel(ButtonCommandId cmdid, s3eKey key, long userdata) :
ButtonPanel(),
m_xSoundFx(eButtonCommandIdSoundFx, s3eKeyFirst),
m_xMusic(eButtonCommandIdMusic, s3eKeyFirst),
m_xReloadLocal(eButtonCommandIdReload, s3eKeyAbsGameB, 0),
m_xReload(eButtonCommandIdReload, s3eKeyAbsGameC, 1) {
}

NaviPanel::~NaviPanel() {
}

void NaviPanel::Initialize() {
	m_xReloadLocal.SetTexture(FactoryManager::GetTextureFactory().Create("button_reload_local"));
	m_xReload.SetTexture(FactoryManager::GetTextureFactory().Create("button_reload"));
	m_xSoundFx.SetTexture(FactoryManager::GetTextureFactory().Create("button_soundfx"));
	m_xMusic.SetTexture(FactoryManager::GetTextureFactory().Create("button_music"));

	AddButton(m_xReloadLocal);
	AddButton(m_xReload);
	AddButton(m_xSoundFx);
	AddButton(m_xMusic);
}

void NaviPanel::OnUpdate(const FrameData& frame) {
	ButtonPanel::OnUpdate(frame);
}

