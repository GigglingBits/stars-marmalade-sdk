#include "TitleScreen.h"
#include "FactoryManager.h"
#include "Debug.h"
#include "SoundEngine.h"
#include "Configuration.h"

TitleScreen::TitleScreen() :
	m_xButtonCredits(eButtonCommandIdCredits, s3eKeyFirst),
    m_xButtonAchievements(eButtonCommandIdAchievements, s3eKeyFirst),
    m_xButtonTitle(eButtonCommandIdOpenWorldMenu, s3eKeyEnter),
    m_xButtonTitleName(eButtonCommandIdOpenLevelMenu, s3eKeyEnter),
    m_xPanelOptions(eButtonCommandIdOptions, s3eKeyFirst) {

	m_pxBackground = FactoryManager::GetTextureFactory().Create("title_bg");

	m_xPanelOptions.StateChanged.AddListener<TitleScreen>(this, &TitleScreen::ButtonPanelStateChangedEventHandler);
}

TitleScreen::~TitleScreen() {
	m_xPanelOptions.StateChanged.RemoveListener<TitleScreen>(this, &TitleScreen::ButtonPanelStateChangedEventHandler);

	if (m_pxBackground) {
		delete m_pxBackground;
	}

	SoundEngine::StopMusicFile();
}

void TitleScreen::Initialize() {
	m_xButtonCredits.SetTexture(FactoryManager::GetTextureFactory().Create("button_credits"));
	m_xButtonAchievements.SetTexture(FactoryManager::GetTextureFactory().Create("button_achievements"));

	m_xPanelOptions.Initialize();
	m_xPanelOptions.GetMainButton().SetTexture(FactoryManager::GetTextureFactory().Create("button_options"));

    m_xButtonTitle.SetTexture(FactoryManager::GetTextureFactory().Create("title_button"));
	m_xButtonTitleName.SetTexture(FactoryManager::GetTextureFactory().Create("title_name"));
    
	SoundEngine::PlayMusicFileLoop(Configuration::GetInstance().IntroSong);
}

void TitleScreen::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xButtonTitle.Update(frame);
	m_xButtonTitleName.Update(frame);
	m_xButtonAchievements.Update(frame);
	m_xButtonCredits.Update(frame);
	m_xPanelOptions.Update(frame);

	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
    }
}

void TitleScreen::OnRender(Renderer& renderer, const FrameData& frame) {
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
	m_xButtonTitleName.Render(renderer, frame);
	m_xButtonTitle.Render(renderer, frame);
	m_xButtonAchievements.Render(renderer, frame);
	m_xButtonCredits.Render(renderer, frame);
	m_xPanelOptions.Render(renderer, frame);
}

void TitleScreen::OnDoLayout(const CIwSVec2& screensize) {
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

	button.x = screencenter.x - (extents / 2) + (1 * button.w);
	button.y = screencenter.y + (extents / 3);
	m_xPanelOptions.GetMainButton().SetPosition(button);
    
	button.x = screencenter.x + (extents / 2) - (2 * button.w);
	m_xButtonAchievements.SetPosition(button);
    
    button.w = (int16)(extents / 1.3f);
    button.h = (int16)(extents / 1.3f);
    button.x = screencenter.x - (button.w / 2);
	button.y = screencenter.y - (button.h / 2);
	m_xButtonTitle.SetPosition(button);
    
    button.w = extents;
    button.h = extents / 10;
    button.x = screencenter.x - (extents / 2);
	button.y = screencenter.y - (extents / 2) + (extents / 12);
	m_xButtonTitleName.SetPosition(button);
}

void TitleScreen::ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args) {
    if (&sender == &m_xButtonTitle) {
		SetCompletionState(eCompleted);
    }
}

void TitleScreen::ButtonPanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args) {
	ChangeButtonState(!args.IsPanelOpen, sender);
}

void TitleScreen::ChangeButtonState(bool enable, const ButtonPanel& except) {
    m_xButtonTitle.SetEnabled(enable);
    m_xButtonTitleName.SetEnabled(enable);
	m_xButtonAchievements.SetEnabled(enable);
	m_xButtonCredits.SetEnabled(enable);
	if (&except != &m_xPanelOptions) m_xPanelOptions.SetEnabled(enable);
}
