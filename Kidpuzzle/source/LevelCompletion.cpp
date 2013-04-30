#include <sstream>

#include "LevelCompletion.h"
#include "Debug.h"
#include "FactoryManager.h"
#include "GameFoundation.h"
#include "Configuration.h"
#include "SoundEngine.h"

LevelCompletion::LevelCompletion(GameFoundation::CompletionInfo& info) : 
	Page(),
	m_xButtonQuit(eButtonCommandIdOpenLevelMenu, s3eKeyAbsGameD) {

	m_bIsCompleted = IsCompleted(info);

	m_pxBackground = FactoryManager::GetTextureFactory().Create("levelstats_bg");
	if (m_pxBackground) {
		m_pxBackground->SelectFrame(m_bIsCompleted ? "won" : "lost");
	}
}

LevelCompletion::~LevelCompletion() {
	if (m_pxBackground) {
		delete m_pxBackground;
	}

	SoundEngine::StopMusicFile();
} 

void LevelCompletion::Initialize() {
	m_xButtonQuit.SetTexture(FactoryManager::GetTextureFactory().Create("button_quit"));

	std::string& song = m_bIsCompleted ? Configuration::GetInstance().WonSong : Configuration::GetInstance().LostSong;
	SoundEngine::PlayMusicFileLoop(song);
}

bool LevelCompletion::IsCompleted(GameFoundation::CompletionInfo& info) {
	return info.IsCleared;
}

void LevelCompletion::OnUpdate(const FrameData& frame) {
	m_xButtonQuit.Update(frame);
	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
    }
}

void LevelCompletion::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	// background
	const CIwSVec2& screen = frame.GetScreensize();
	CIwRect rect(0, 0, screen.x, screen.y);
    if (m_pxBackground) {
        VertexStreamScreen shape;
        shape.SetRect(rect);
        renderer.Draw(shape, *m_pxBackground);
    }

	m_xButtonQuit.Render(renderer, frame);
}

void LevelCompletion::OnDoLayout(const CIwSVec2& screensize) {
	int extents = GetScreenExtents();
	int margin = extents / 4;	// 25% 
	int space = margin / 5;		// 25% / 5 = 5% 
		
	// the button size is determined by the screen height	
	CIwRect button;
	button.h = extents / 7;	// height
	button.w = button.h;	// width

	// positions
	button.x = (screensize.x - (3 * button.w) - (2 * space)) / 2;
	button.y = screensize.y * 10 / 13;
	m_xButtonQuit.SetPosition(button);
}
