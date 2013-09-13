#include <sstream>

#include "LevelCompletion.h"
#include "Debug.h"
#include "FactoryManager.h"
#include "GameFoundation.h"
#include "Configuration.h"
#include "SoundEngine.h"

LevelCompletion::LevelCompletion(GameFoundation::CompletionInfo& info) :
    m_xButtonStar(eButtonCommandIdNone, s3eKeyFirst),
    m_xButtonQuit(eButtonCommandIdOpenLevelMenu, s3eKeyAbsGameD),
	m_xButtonRetry(eButtonCommandIdRestartLevel, s3eKeyAbsGameB),
	m_xButtonNext(eButtonCommandIdOpenNextLevel, s3eKeyFirst) {

	m_bIsCompleted = IsCompleted(info);
	m_sCompletionText = GenerateCompletionText(info);

		m_xDustFillPercent.SetNumber(info.DustFillPercent * 100.0f, 5000);
		
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
	m_xButtonStar.SetTexture(FactoryManager::GetTextureFactory().Create("button_completion"));
    m_xButtonStar.SetTextureFrame(m_bIsCompleted ? "won" : "lost");
    
	m_xButtonQuit.SetTexture(FactoryManager::GetTextureFactory().Create("button_quit"));
	m_xButtonNext.SetTexture(FactoryManager::GetTextureFactory().Create("button_next"));
	m_xButtonRetry.SetTexture(FactoryManager::GetTextureFactory().Create("button_restart"));

	m_xButtonNext.SetEnabled(m_bIsCompleted);

	std::string& song = m_bIsCompleted ? Configuration::GetInstance().WonSong : Configuration::GetInstance().LostSong;
	SoundEngine::PlayMusicFileLoop(song);
}

bool LevelCompletion::IsCompleted(GameFoundation::CompletionInfo& info) {
	return info.IsCleared;
}

std::string LevelCompletion::GenerateCompletionText(GameFoundation::CompletionInfo& info) {
	std::ostringstream oss;

	if (info.IsCleared) {
		oss << "The level is cleared" << std::endl;
	} else {
		oss << "Try again" << std::endl;
	}

	return oss.str();
}

void LevelCompletion::OnUpdate(const FrameData& frame) {
	m_xButtonStar.Update(frame);
	m_xButtonQuit.Update(frame);
	m_xButtonRetry.Update(frame);
	m_xButtonNext.Update(frame);

	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
    }
	
	m_xDustFillPercent.Update(frame);
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

	// text
	rect.y = 0;
	rect.h = screen.y / 2;
	renderer.DrawText(m_sCompletionText, rect);
	
	m_xDustFillPercent.Render(renderer, frame);

	// buttons
	m_xButtonStar.Render(renderer, frame);
	m_xButtonQuit.Render(renderer, frame);
	m_xButtonRetry.Render(renderer, frame);
	m_xButtonNext.Render(renderer, frame);
}

void LevelCompletion::OnDoLayout(const CIwSVec2& screensize) {
	int extents = GetScreenExtents();
	int margin = extents / 4;	// 25% 
	int space = margin / 5;		// 25% / 5 = 5% 
	CIwSVec2 screencenter(screensize.x / 2, screensize.y / 2);
	
	// star button
	CIwRect button;
	button.h = extents / 2;
	button.w = button.h;
	button.x = screencenter.x - (button.h / 2);
	button.y = screencenter.y - (button.w / 2);
	m_xButtonStar.SetPosition(button);
    
	// text
	m_xDustFillPercent.SetPosition(CIwSVec2(150, 250));
	m_xDustFillPercent.SetSize(CIwSVec2(150, 50));
	
    // the button size is determined by the screen height
	button.h = extents / 7;
	button.w = button.h;
    
	// positions
	button.x = (screensize.x - (3 * button.w) - (2 * space)) / 2;
	button.y = screensize.y * 10 / 13;
	m_xButtonQuit.SetPosition(button);

	button.x += button.w + space;
	m_xButtonRetry.SetPosition(button);

	button.x += button.w + space;
	m_xButtonNext.SetPosition(button);
}
