#include "LevelPausedPanel.h"
#include "FactoryManager.h"
#include "Configuration.h"

#include "Debug.h"

LevelPausedPanel::LevelPausedPanel(ButtonCommandId cmdid, s3eKey key, long userdata) :
ButtonPanel(cmdid, key, userdata),
m_xBtnQuit(eButtonCommandIdQuitLevel, s3eKeyAbsGameD),
m_xBtnRestart(eButtonCommandIdRestartLevel, s3eKeyR),
m_xBtnReloadLocal(eButtonCommandIdReload, s3eKeyL, 0),
m_xBtnReload(eButtonCommandIdReload, s3eKeyD, 1) {
	SetAlpha(0x66);
}

void LevelPausedPanel::Initialize() {
	uint32 textcol = 0xffccfaff;

	m_xBtnRestart.SetText("Restart", textcol);
	m_xBtnRestart.SetTexture(FactoryManager::GetTextureFactory().Create("button_pause_restart"));
	AddButton(m_xBtnRestart);
	
	m_xBtnQuit.SetText("Quit", textcol);
	m_xBtnQuit.SetTexture(FactoryManager::GetTextureFactory().Create("button_pause_quit"));
	AddButton(m_xBtnQuit);

	if (Configuration::GetInstance().UnlockAll) {
		m_xBtnReload.SetText("Reload Dropbox", textcol);
		m_xBtnReload.SetTexture(FactoryManager::GetTextureFactory().Create("button_pause_reload"));
		AddButton(m_xBtnReload);
	
		m_xBtnReloadLocal.SetText("Reload Local", textcol);
		m_xBtnReloadLocal.SetTexture(FactoryManager::GetTextureFactory().Create("button_pause_reload_local"));
		AddButton(m_xBtnReloadLocal);
	}
}

void LevelPausedPanel::OnDoLayout(const CIwSVec2& screensize) {
	int extents = GetScreenExtents();
	int width = extents / 2;
	int height = width / 2;
	int space = height / 20;
	
	CIwRect rect(0, 0, width, height);
	int count = GetButtonCount();
	for (int i = 0; i < count; i++) {
		if (Button* p = GetButton(i)) {
			rect.x = (screensize.x - width) / 2;
			rect.y = (screensize.y - (count * (height + space) - space)) / 2 + (i * (height + space));
			p->SetPosition(rect);
		}
	}
}
