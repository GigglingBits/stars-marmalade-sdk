#include "Level.h"

#include <string>

#include "Debug.h"
#include "Configuration.h"
#include "InputManager.h"
#include "SoundEngine.h"
#include "FactoryManager.h"

Level::Level(const CIwFVec2& worldsize, std::string background) : 
	m_bIsPaused(false),
	m_xBackground(background), 
	m_xGame(worldsize), 
	m_xInteractor(m_xCamera, m_xGame),
	m_xAimingDevice(m_xGame),
	m_iCompletionTimer(0),
	m_xAppPanel(eButtonCommandIdToggleHud, s3eKeyP) {

	// attach event handlers
	s3eDeviceRegister(S3E_DEVICE_PAUSE, AppPausedCallback, this);
	m_xGame.SoundEffectEvent.AddListener<Level>(this, &Level::SoundEffectEventHandler);
	m_xGame.HeroChangedEvent.AddListener<Level>(this, &Level::HeroChangedEventHandler);
	m_xAppPanel.StateChanged.AddListener<Level>(this, &Level::ButtonPanelStateChangedEventHandler);
	m_xStatsPanel.HeroFocusRequested.AddListener<Level>(this, &Level::HeroFocusChangedEventHandler);

	// set up the optics accorting to world size (e.g. according to level definition)
	CIwSVec2 screensize(IwGxGetDisplayWidth(), IwGxGetDisplayHeight());
	float maxvisibleworldsize = (float)Configuration::GetInstance().MaxVisibleWorldSize;
	m_xCamera.SetGeometry(worldsize, screensize, maxvisibleworldsize);

	// create background image to fit the optics
	float margin = (float) Configuration::GetInstance().WorldMargin;
	m_xBackground.SetGeometry(worldsize, screensize, margin);
}

Level::~Level() {
	// detach event handlers
	m_xStatsPanel.HeroFocusRequested.RemoveListener<Level>(this, &Level::HeroFocusChangedEventHandler);
	m_xAppPanel.StateChanged.RemoveListener<Level>(this, &Level::ButtonPanelStateChangedEventHandler);
	m_xGame.SoundEffectEvent.RemoveListener<Level>(this, &Level::SoundEffectEventHandler);
	m_xGame.HeroChangedEvent.RemoveListener<Level>(this, &Level::HeroChangedEventHandler);
	s3eDeviceUnRegister(S3E_DEVICE_PAUSE, AppPausedCallback);

	SoundEngine::StopMusicFile();
}

void Level::Initialize() {
	m_xAppPanel.Initialize();
	m_xAppPanel.GetMainButton().SetTexture(FactoryManager::GetTextureFactory().Create("button_toggle_hud"));

	m_xStatsPanel.Initialize();

	SoundEngine::PlayMusicFileLoop("music/levelsong.mp3");
}

void Level::Add(Body* body) {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, body, ("Could not add body to level. Seems to be NULL..."));
	if (body) {
		m_xGame.Add(body);
	}
}

void Level::SetPaused() {
	m_xAppPanel.ShowPanel();
	IwAssertMsg(MYAPP, m_bIsPaused, ("Pause requested, but did not work."));
}

void Level::SetHeroFocus(const std::string& heroid) {
	m_sHeroInFocus = heroid;
}

void Level::ClearHeroFocus() {
	m_sHeroInFocus.clear();
}

bool Level::IsHeroInFocus() {
	return !m_sHeroInFocus.empty();
}

bool Level::GetCompletionState(LevelCompletionInfo& info) {
	if (Page::GetCompletionState() == eCompleted) {
		m_xGame.GetCompletionInfo(info);
		return true;
	}
	return false;
}

void Level::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xAppPanel.Update(frame);

	if (m_bIsPaused) {
		return;
	}

	// update game logic (remove dead, etc...)
	m_xGame.Update(frame);

	// check if level is finished
	if (m_xGame.IsCompleted()) {
		// delay the completion for a short time
		m_iCompletionTimer += frame.GetSimulatedDurationMs();
		if (LEVEL_COMPLETION_DELAY < m_iCompletionTimer) {
			SetCompletionState(eCompleted);
		}
	}

	// find hero with focus
	if (m_xCamera.IsPanning()) {
		ClearHeroFocus();
	} else if (m_xInteractor.IsHeroInFocus()) {
		SetHeroFocus(m_xInteractor.GetHeroInFocus());
	}
	Body* hero = NULL;
	if (IsHeroInFocus()) {
		hero = m_xGame.FindSprite<Body>(m_sHeroInFocus);
	}

	if (hero) {
		// set up aiming device
		m_xAimingDevice.Show();
		m_xAimingDevice.SetPosition(hero->GetCenter());
		if (m_xInteractor.IsHeroCharging()) {
			LevelInteractor::HeroChargeInfo info = m_xInteractor.GetHeroChargeInfo();
			IwAssert(MYAPP, hero->GetId() == info.heroid);
			m_xAimingDevice.SetVelocity(info.impulse, hero->GetMass());
		} else {
			m_xAimingDevice.SetVelocity(CIwFVec2::g_Zero, 1.0f);
		}

		// move camera along
		m_xCamera.SetWorldFocus(hero->GetCenter());
	} else {
		m_xAimingDevice.Hide();
	}

	// scene and widgets
	m_xCamera.Update(frame.GetScreensize(), frame.GetSimulatedDurationMs());
	m_xBackground.Update(frame);
	m_xAimingDevice.Update(frame);
	m_xStatsPanel.Update(frame);
}

void Level::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.SetViewport(m_xCamera.GetViewport());
	m_xBackground.Render(renderer, frame);
	m_xGame.Render(renderer, frame);
	m_xAimingDevice.Render(renderer, frame);

	m_xStatsPanel.Render(renderer, frame);
	m_xAppPanel.Render(renderer, frame);

	// visualize the pinch
	if (/*Configuration::GetInstance().ShowStats &&*/ m_xInteractor.IsPinching()) {
		const PinchGesture::PinchInfo& pinch = m_xInteractor.GetPinchInfo();
		CIwSVec2 center((int16)pinch.currentcenter.x, (int16)pinch.currentcenter.y);
		CIwSVec2 correction((int16)pinch.currentcentercorrection.x, (int16)pinch.currentcentercorrection.y);
		center -= correction;
		renderer.DrawRect(CIwRect(center.x - 10, center.y - 10, 20, 20), 0xffffffff, 0x55ffffff);

		CIwSVec2 movement((int16)pinch.movement.x, (int16)pinch.movement.y);
		renderer.DrawLine(center - movement, center, 0xffffffff);

		static char buf[20];
		snprintf(buf, 20, "ratio: %.2f", pinch.scalefactor);
		renderer.DrawText(std::string(buf), center, Renderer::eFontTypeSystem, 0xffffffff);
	}
}

void Level::OnDoLayout(const CIwSVec2& screensize) {
	uint32 width = IwGxGetScreenWidth();
	uint32 size = 60;
	uint32 margin = 15;
	m_xAppPanel.GetMainButton().SetPosition(CIwRect(width - (size + margin), margin, size, size));
}

CIwFVec2 Level::CalculateRelativeSoundPosition(const CIwFVec2& worldpos) {
	CIwSVec2 soundpixelpos = m_xCamera.GetViewport().WorldToView(worldpos);
	CIwSVec2 centeroffset(IwGxGetScreenWidth() / 2, IwGxGetScreenHeight() / 2);
	soundpixelpos -= centeroffset;
	return CIwFVec2(soundpixelpos.x / (float)centeroffset.x, soundpixelpos.y / (float)centeroffset.y);
}

void Level::HeroChangedEventHandler(const GameFoundation& sender, const GameFoundation::HeroChangedEventArgs& args) {
	if (args.changetype == GameFoundation::eHeroChangeKilled) {
		m_xStatsPanel.Kill(args.id);
	} else if (args.changetype == GameFoundation::eHeroChangeRescued) {
		m_xStatsPanel.Rescue(args.id);
	} else if (args.changetype == GameFoundation::eHeroChangeBorn) {
		m_xStatsPanel.Add(args.id);
		SetHeroFocus(args.id);
	} else {
		IwAssertMsg(MYAPP, false, ("Unknown hero state changed."));
	}
}

void Level::SoundEffectEventHandler(const GameFoundation& sender, const GameFoundation::SoundEventArgs& args) {
	// locate the sound, relative to the view;
	// normalize: from >-1 .. <1 is on screen
	CIwFVec2 pan = CalculateRelativeSoundPosition(args.position);
	IwTrace(MYAPP, ("Sounds: %s; impulse=%f; pan=%f/%f", args.soundgroup.c_str(), args.impulse, pan.x, pan.y));

	SoundEngine::PlaySoundEffect("collision");
}

void Level::ButtonPanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args) {
	m_bIsPaused = !args.IsOpen;
	m_xStatsPanel.SetEnabled(!m_bIsPaused);
}

int32 Level::AppPausedCallback(void* systemData, void* userData) {
	IW_CALLSTACK_SELF;

	Level* lvl = (Level*) userData;
	lvl->SetPaused();
	return 0;
}

void Level::HeroFocusChangedEventHandler(const LevelStatsPanel& sender, const std::string& heroid) {
	SetHeroFocus(heroid);
}
