#include "TitleScreen.h"
#include "FactoryManager.h"
#include "Debug.h"
#include "Configuration.h"
#include "LogManager.h"
#include "Leaderboards.h"
#include "AppAnalytics.h"

#include "s3eOsExec.h"
#include "s3eVideo.h"

#include "spine/extension.h"

TitleScreen::TitleScreen() :
Page("menu.group", Configuration::GetInstance().MenuSong),
m_xButtonTitle(eButtonCommandIdOpenWorldMenu, s3eKeyAbsOk),
m_xButtonFacebook(eButtonCommandIdFacebook, s3eKeyF),
m_xButtonLeaderboards(eButtonCommandIdLeaderboards, s3eKeyL),
m_xButtonMovie(eButtonCommandIdOpenIntroMovie, s3eKeyM),
m_xBackButton(eButtonCommandIdNone, s3eKeyAbsGameD),
m_bHasFacebookButton(false),
m_bHasLeaderboardsButton(false),
m_bHasMovieButton(false) {
	m_xButtonFacebook.PressedEvent.AddListener<TitleScreen>(this, &TitleScreen::ButtonPressedEventHandler);
	m_xButtonLeaderboards.PressedEvent.AddListener<TitleScreen>(this, &TitleScreen::ButtonPressedEventHandler);
	m_xBackButton.PressedEvent.AddListener(this, &TitleScreen::ButtonReleasedEventHandler);
}

TitleScreen::~TitleScreen() {
	m_xBackButton.PressedEvent.RemoveListener(this, &TitleScreen::ButtonReleasedEventHandler);
	m_xButtonLeaderboards.PressedEvent.RemoveListener<TitleScreen>(this, &TitleScreen::ButtonPressedEventHandler);
	m_xButtonFacebook.PressedEvent.RemoveListener<TitleScreen>(this, &TitleScreen::ButtonPressedEventHandler);
}

void TitleScreen::Initialize() {
	IW_CALLSTACK_SELF;
	
	m_xCamera.SetGeometry(CIwFVec2(15.0f, 5.0f), CIwSVec2(IwGxGetScreenWidth(), IwGxGetScreenHeight()), 10.0f);
	m_xCamera.SetWorldFocus(CIwFVec2(0.0f, 0.0f));
	m_xCamera.ZoomOut();
	
	m_xBackground.Initialize();

    m_xButtonTitle.SetTexture(FactoryManager::GetTextureFactory().Create("title_button"));
	m_xButtonTitle.SetTextureFrame("enter");
	
	m_bHasFacebookButton = s3eOSExecAvailable();
	m_bHasFacebookButton &= !Configuration::GetInstance().FacebookPage.empty();
	m_bHasFacebookButton |= Configuration::GetInstance().UnlockAll;
	if (m_bHasFacebookButton) {
		m_xButtonFacebook.SetTexture(FactoryManager::GetTextureFactory().Create("button_facebook"));
	}
	
	m_bHasLeaderboardsButton = Leaderboards::GetInstance().IsAvailable();
	m_bHasLeaderboardsButton &= !Configuration::GetInstance().LeaderboardKey.empty();
	m_bHasLeaderboardsButton |= Configuration::GetInstance().UnlockAll;
	if (m_bHasLeaderboardsButton) {
		m_xButtonLeaderboards.SetTexture(FactoryManager::GetTextureFactory().Create("button_leaderboards"));
	}
	
	m_bHasMovieButton = s3eVideoGetInt(S3E_VIDEO_AVAILABLE) != 0;
	m_bHasMovieButton &= !Configuration::GetInstance().IntroMovie.empty();
	m_bHasMovieButton &= s3eVideoIsCodecSupported(S3E_VIDEO_CODEC_MPEG4_VIDEO_MPEG4);
	m_bHasMovieButton &= s3eVideoIsCodecSupported(S3E_VIDEO_CODEC_MPEG4_AUDIO_AAC);
	m_bHasMovieButton |= Configuration::GetInstance().UnlockAll;
	if (m_bHasMovieButton) {
		m_xButtonMovie.SetTexture(FactoryManager::GetTextureFactory().Create("button_movie"));
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
	button.w = extents / 7;
	button.h = extents / 7;
	button.y = screensize.y - button.h - (button.h / 4);
	if (m_bHasFacebookButton) {
		button.x = screensize.x - button.w - (button.w / 4);
		m_xButtonFacebook.SetPosition(button);
	}
	if (m_bHasLeaderboardsButton) {
		button.x -= button.w + (button.w / 4);
		m_xButtonLeaderboards.SetPosition(button);
	}
	if (m_bHasMovieButton) {
		button.x = button.w / 4;
		m_xButtonMovie.SetPosition(button);
	}
}

void TitleScreen::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xButtonTitle.Update(frame);
	if (m_bHasFacebookButton) {
		m_xButtonFacebook.Update(frame);
	}
	if (m_bHasLeaderboardsButton) {
		m_xButtonLeaderboards.Update(frame);
	}
	if (m_bHasMovieButton) {
		m_xButtonMovie.Update(frame);
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
	if (m_bHasLeaderboardsButton) {
		m_xButtonLeaderboards.Render(renderer, frame);
	}	
	if (m_bHasMovieButton) {
		m_xButtonMovie.Render(renderer, frame);
	}
}

void TitleScreen::ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args) {
    if (&sender == &m_xButtonTitle) {
		SetCompletionState(eCompleted);
	} else if (&sender == &m_xButtonFacebook) {
		OpenFacebook();
	} else if (&sender == &m_xButtonLeaderboards) {
		OpenLeaderboards();
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

void TitleScreen::OpenLeaderboards() {
	if (!Leaderboards::GetInstance().ShowLeaderboard(Configuration::GetInstance().LeaderboardKey)) {
		IwError(("Could not open leaderboard UI"));
	}
	
	AppAnalytics a;
	a.RegisterLeaderboardsOpened();
}

void TitleScreen::ButtonReleasedEventHandler(const InputManager::VirtualButton& sender, const InputManager::VirtualButton::EventArgs& args) {
	if (sender.key == s3eKeyAbsGameD) {
		s3eDeviceRequestQuit();
		args.handled = true;
	}
}
