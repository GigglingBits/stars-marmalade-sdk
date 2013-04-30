#include "TitleMenu.h"
#include "FactoryManager.h"
#include "Debug.h"
#include "SoundEngine.h"
#include "Configuration.h"

TitleMenu::TitleMenu() : 
	Page(),
	m_xButtonPlay(eButtonCommandIdOpenLevelMenu, s3eKeyFirst) {
	m_pxBackground = FactoryManager::GetTextureFactory().Create("titlemenu_bg");
}

TitleMenu::~TitleMenu() {
	if (m_pxBackground) {
		delete m_pxBackground;
	}

	SoundEngine::StopMusicFile();
}

void TitleMenu::Initialize() {
	m_xButtonPlay.SetTexture(FactoryManager::GetTextureFactory().Create("button_play"));
	SoundEngine::PlayMusicFileLoop(Configuration::GetInstance().IntroSong);
}

void TitleMenu::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xButtonPlay.Update(frame);

	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
    }
}

void TitleMenu::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.SetViewport(m_xCamera.GetViewport());

    // background
    if (m_pxBackground) {
        VertexStreamScreen shape;        
        const CIwSVec2& screensize = frame.GetScreensize();
        shape.SetRect(CIwRect(0, 0, screensize.x, screensize.y));
        renderer.Draw(shape, *m_pxBackground);
    }
    
    // buttons
	m_xButtonPlay.Render(renderer, frame);
}

void TitleMenu::OnDoLayout(const CIwSVec2& screensize) {
	int extents = GetScreenExtents();


	// button size
	CIwRect button;
	button.h = extents / 7;						
	button.w = button.h;						

	// location of the buttons
	CIwSVec2 screencenter(screensize.x / 2, screensize.y / 2);
	button.x = screencenter.x + (extents / 2) - (1 * button.w);
	button.y = screencenter.y + button.h;
	m_xButtonPlay.SetPosition(button);
}
