#include "TitleMenu.h"
#include "FactoryManager.h"
#include "Debug.h"
#include "SoundEngine.h"

TitleMenu::TitleMenu() : 
	Page(),
	m_xButtonPlay(eButtonCommandIdOpenLevelMenu, s3eKeyFirst),
	m_xPanelOptions(eButtonCommandIdOptions, s3eKeyFirst),
	m_xPanelSocial(eButtonCommandIdSocial, s3eKeyFirst) {

	m_pxBackground = FactoryManager::GetTextureFactory().Create("titlemenu_bg");

	m_xPanelOptions.StateChanged.AddListener<TitleMenu>(this, &TitleMenu::ButtonPanelStateChangedEventHandler);
	m_xPanelSocial.StateChanged.AddListener<TitleMenu>(this, &TitleMenu::ButtonPanelStateChangedEventHandler);
}

TitleMenu::~TitleMenu() {
	m_xPanelOptions.StateChanged.RemoveListener<TitleMenu>(this, &TitleMenu::ButtonPanelStateChangedEventHandler);
	m_xPanelSocial.StateChanged.RemoveListener<TitleMenu>(this, &TitleMenu::ButtonPanelStateChangedEventHandler);

	if (m_pxBackground) {
		delete m_pxBackground;
	}
}

void TitleMenu::Initialize() {
	m_xButtonPlay.SetTexture(FactoryManager::GetTextureFactory().Create("button_play"));

	m_xPanelOptions.Initialize();
	m_xPanelOptions.GetMainButton().SetTexture(FactoryManager::GetTextureFactory().Create("button_options"));

	m_xPanelSocial.Initialize();
	m_xPanelSocial.GetMainButton().SetTexture(FactoryManager::GetTextureFactory().Create("button_social"));
}

void TitleMenu::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	// update buttons
	m_xButtonPlay.Update(frame);

	if (m_xPanelSocial.IsPanelOpen() && m_xPanelOptions.IsEnabled()) {
		m_xPanelOptions.SetEnabled(false);
	} else if (!m_xPanelSocial.IsPanelOpen() && !m_xPanelOptions.IsEnabled()) {
		m_xPanelOptions.SetEnabled(true);
	}
	if (m_xPanelOptions.IsPanelOpen() && m_xPanelSocial.IsEnabled()) {
		m_xPanelSocial.SetEnabled(false);
	} else if (!m_xPanelOptions.IsPanelOpen() && !m_xPanelSocial.IsEnabled()) {
		m_xPanelSocial.SetEnabled(true);
	}
	m_xPanelOptions.Update(frame);
	m_xPanelSocial.Update(frame);

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
	m_xPanelOptions.Render(renderer, frame);
	m_xPanelSocial.Render(renderer, frame);
}

void TitleMenu::OnDoLayout(const CIwSVec2& screensize) {
	int extents = GetScreenExtents();
	int margin = extents / 4;	// 25% 
	int space = margin / 5;		// 25% / 5 = 5% 

	// size of PLAY button is determined by the screen height	
	CIwRect button;
	button.h = extents / 5;				// height
	button.w = 2 * button.h;			// width

	button.x = (screensize.x - button.w) / 2;	// left
	button.y = screensize.y / 2;				// top btn1
	m_xButtonPlay.SetPosition(button);

	// size of other buttons
	button.h = extents / 7;	// height
	button.w = button.h;	// width

	button.x = (screensize.x - (3 * button.w) - (2 * space)) / 2;
	button.y = screensize.y * 10 / 13;
	m_xPanelOptions.GetMainButton().SetPosition(button);

	button.x += button.w + space;
	//m_xButtonOptions.SetPosition(button);

	button.x += button.w + space;
	m_xPanelSocial.GetMainButton().SetPosition(button);
}

void TitleMenu::ButtonPanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args) {
	ChangeState(args.IsPanelOpen, sender);
}

void TitleMenu::ChangeState(bool enable, const ButtonPanel& except) {
	m_xButtonPlay.SetEnabled(enable);
	if (&except != &m_xPanelOptions) m_xPanelOptions.SetEnabled(enable);
	if (&except != &m_xPanelSocial) m_xPanelSocial.SetEnabled(enable);
}
