#include "TitleMenu.h"
#include "FactoryManager.h"
#include "Debug.h"
#include "SoundEngine.h"
#include "Configuration.h"

TitleMenu::TitleMenu() : 
	Page(),
	m_xButtonCredits(eButtonCommandIdCredits, s3eKeyFirst),
	m_xButtonAchievements(eButtonCommandIdAchievements, s3eKeyFirst),
	m_xButtonPlay(eButtonCommandIdOpenLevelMenu, s3eKeyFirst),
	m_xPanelOptions(eButtonCommandIdOptions, s3eKeyFirst) {

	m_pxBackground = FactoryManager::GetTextureFactory().Create("titlemenu_bg");

	m_xPanelOptions.StateChanged.AddListener<TitleMenu>(this, &TitleMenu::ButtonPanelStateChangedEventHandler);
}

TitleMenu::~TitleMenu() {
	m_xPanelOptions.StateChanged.RemoveListener<TitleMenu>(this, &TitleMenu::ButtonPanelStateChangedEventHandler);

	if (m_pxBackground) {
		delete m_pxBackground;
	}

	SoundEngine::StopMusicFile();
}

void TitleMenu::Initialize() {
	m_xButtonPlay.SetTexture(FactoryManager::GetTextureFactory().Create("button_play"));
	m_xButtonCredits.SetTexture(FactoryManager::GetTextureFactory().Create("button_credits"));
	m_xButtonAchievements.SetTexture(FactoryManager::GetTextureFactory().Create("button_achievements"));

	m_xPanelOptions.Initialize();
	m_xPanelOptions.GetMainButton().SetTexture(FactoryManager::GetTextureFactory().Create("button_options"));

	SoundEngine::PlayMusicFileLoop(Configuration::GetInstance().IntroSong);
}

void TitleMenu::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xButtonPlay.Update(frame);
	m_xButtonAchievements.Update(frame);
	m_xButtonCredits.Update(frame);
	m_xPanelOptions.Update(frame);

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
	m_xButtonAchievements.Render(renderer, frame);
	m_xButtonCredits.Render(renderer, frame);
	m_xPanelOptions.Render(renderer, frame);
}

void TitleMenu::OnDoLayout(const CIwSVec2& screensize) {
	int extents = GetScreenExtents();


	// button size
	CIwRect button;
	button.h = extents / 7;						
	button.w = button.h;						

	// location of the buttons
	CIwSVec2 screencenter(screensize.x / 2, screensize.y / 2);
	button.x = screencenter.x - (extents / 2) ;
	button.y = screencenter.y + button.h;
	m_xButtonCredits.SetPosition(button);

	button.x = screencenter.x + (extents / 2) - (1 * button.w);
	m_xButtonPlay.SetPosition(button);

	button.x = screencenter.x - (extents / 2) + (1 * button.w);
	button.y = screencenter.y + (extents / 3);
	m_xPanelOptions.GetMainButton().SetPosition(button);

	button.x = screencenter.x + (extents / 2) - (2 * button.w);
	m_xButtonAchievements.SetPosition(button);
}

void TitleMenu::ButtonPanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args) {
	ChangeState(args.IsPanelOpen, sender);
}

void TitleMenu::ChangeState(bool enable, const ButtonPanel& except) {
	m_xButtonPlay.SetEnabled(enable);
	m_xButtonAchievements.SetEnabled(enable);
	m_xButtonCredits.SetEnabled(enable);
	if (&except != &m_xPanelOptions) m_xPanelOptions.SetEnabled(enable);
}
