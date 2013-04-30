#include "OptionsPanel.h"
#include "FactoryManager.h"

OptionsPanel::OptionsPanel(ButtonCommandId cmdid, s3eKey key, long userdata) : 
ButtonPanel(cmdid, key, userdata),
m_xSoundFx(eButtonCommandIdSoundFx, s3eKeyFirst),
m_xMusic(eButtonCommandIdMusic, s3eKeyFirst),
m_xHelp(eButtonCommandIdHelp, s3eKeyFirst),
m_xReloadLocal(eButtonCommandIdReload, s3eKeyAbsGameB, 0),
m_xReload(eButtonCommandIdReload, s3eKeyAbsGameC, 1) {
	m_xDemoTimer.ElapsedEvent.AddListener<OptionsPanel>(this, &OptionsPanel::DemoTimerElapsedEventHandler);
}

OptionsPanel::~OptionsPanel() {
	m_xDemoTimer.ElapsedEvent.RemoveListener<OptionsPanel>(this, &OptionsPanel::DemoTimerElapsedEventHandler);
}

void OptionsPanel::Initialize() {
	m_xReloadLocal.SetTexture(FactoryManager::GetTextureFactory().Create("button_reload_local"));
	m_xReload.SetTexture(FactoryManager::GetTextureFactory().Create("button_reload"));
	m_xSoundFx.SetTexture(FactoryManager::GetTextureFactory().Create("button_soundfx"));
	m_xMusic.SetTexture(FactoryManager::GetTextureFactory().Create("button_music"));
	m_xHelp.SetTexture(FactoryManager::GetTextureFactory().Create("button_help"));

	AddButton(m_xReloadLocal);
	AddButton(m_xReload);
	AddButton(m_xSoundFx);
	AddButton(m_xMusic);
	AddButton(m_xHelp);

	m_xDemoTimer.Start(1000, 0);
}

void OptionsPanel::OnUpdate(const FrameData& frame) {
	ButtonPanel::OnUpdate(frame);
	m_xDemoTimer.Update(frame.GetRealDurationMs());
}

void OptionsPanel::DemoTimerElapsedEventHandler(const Timer& sender, const long& data) {
	// toggle button textures for demo purposes
	static bool state = false;
	state = !state;
	if (state) {
		m_xSoundFx.SetTexture(FactoryManager::GetTextureFactory().Create("button_soundfx"));
		m_xMusic.SetTexture(FactoryManager::GetTextureFactory().Create("button_music"));
	} else {
		m_xSoundFx.SetTexture(FactoryManager::GetTextureFactory().Create("button_soundfx_mute"));
		m_xMusic.SetTexture(FactoryManager::GetTextureFactory().Create("button_music_mute"));
	}

	m_xDemoTimer.Start(1000, 0);
}
