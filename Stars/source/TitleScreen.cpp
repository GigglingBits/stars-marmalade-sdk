#include "TitleScreen.h"
#include "FactoryManager.h"
#include "Debug.h"
#include "Configuration.h"
#include "LogManager.h"
#include "AppAnalytics.h"

#include "s3eOsExec.h"

#include "spine/extension.h"

TitleScreen::TitleScreen() :
Page("menu.group", Configuration::GetInstance().MenuSong),
m_xButtonTitle(eButtonCommandIdOpenWorldMenu, s3eKeyAbsOk),
m_xButtonFacebook(eButtonCommandIdFacebook, s3eKeyF),
m_bHasFacebookButton(false) {
	m_xButtonFacebook.PressedEvent.AddListener<TitleScreen>(this, &TitleScreen::ButtonPressedEventHandler);
}

TitleScreen::~TitleScreen() {
	m_xButtonFacebook.PressedEvent.RemoveListener<TitleScreen>(this, &TitleScreen::ButtonPressedEventHandler);
}

void TitleScreen::Initialize() {
	IW_CALLSTACK_SELF;

	m_bHasFacebookButton = s3eOSExecAvailable() && !Configuration::GetInstance().FacebookPage.empty();
	
	m_xCamera.SetGeometry(CIwFVec2(15.0f, 5.0f), CIwSVec2(IwGxGetScreenWidth(), IwGxGetScreenHeight()), 10.0f);
	m_xCamera.SetWorldFocus(CIwFVec2(0.0f, 0.0f));
	m_xCamera.ZoomOut();
	
	m_xBackground.Initialize();

    m_xButtonTitle.SetTexture(FactoryManager::GetTextureFactory().Create("title_button"));
	m_xButtonTitle.SetTextureFrame("enter");
	
	if (m_bHasFacebookButton) {
		m_xButtonFacebook.SetTexture(FactoryManager::GetTextureFactory().Create("button_facebook"));
	}
}

void TitleScreen::OnDoLayout(const CIwSVec2& screensize) {
	CIwSVec2 screencenter(screensize.x / 2, screensize.y / 2);
	CIwRect button;
	int extents = GetScreenExtents();
	
	// main button
    button.w = extents;
    button.h = button.w * 3 / 5;
    button.x = screencenter.x - (button.w / 2);
	button.y = extents / 5;
	m_xButtonTitle.SetPosition(button);
	
    // menu buttons
	if (m_bHasFacebookButton) {
		button.w = extents / 7;
		button.h = extents / 7;
		button.x = screensize.x - button.w - (button.w / 4);
		button.y = screensize.y - button.h - (button.h / 4);
		m_xButtonFacebook.SetPosition(button);
	}
}

void TitleScreen::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xButtonTitle.Update(frame);
	if (m_bHasFacebookButton) {
		m_xButtonFacebook.Update(frame);
	}
	m_xBackground.Update(frame);
}

void TitleScreen::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.SetViewport(m_xCamera.GetViewport());
	m_xBackground.Render(renderer, frame);
	
	// buttons
	m_xButtonTitle.Render(renderer, frame);
	if (m_bHasFacebookButton) {
		m_xButtonFacebook.Render(renderer, frame);
	}
}

void TitleScreen::ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args) {
    if (&sender == &m_xButtonTitle) {
		SetCompletionState(eCompleted);
    } else if (&sender == &m_xButtonFacebook) {
		OpenFacebook();
	}
}

void TitleScreen::OpenFacebook() {
	s3eResult res = s3eOSExecExecute(Configuration::GetInstance().FacebookPage.c_str(), false);
	if (res != S3E_RESULT_SUCCESS) {
		std::string msg = "I am very sorry. Something went wrong. I cannot open Facebook for you. But you can find us here: " + Configuration::GetInstance().FacebookPage;
		LogManager::GetInstance().WriteMessage(msg);
	}

	AppAnalytics a;
	a.RegisterFacebookOpened();
}
