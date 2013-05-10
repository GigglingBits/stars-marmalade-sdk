#include "Level.h"

#include <string>

#include "Debug.h"
#include "Configuration.h"
#include "InputManager.h"
#include "SoundEngine.h"
#include "FactoryManager.h"

Level::Level(const CIwFVec2& worldsize, std::string background) :
	m_bIsPaused(false),
	m_xWorldSize(worldsize),
	m_xGame(worldsize), 
	m_xBackground(background, m_xGame), 
	m_xInteractor(m_xCamera, m_xGame),
	m_iCompletionTimer(0),
	m_xAppPanel(eButtonCommandIdToggleHud, s3eKeyP) {

	// attach event handlers
	s3eDeviceRegister(S3E_DEVICE_PAUSE, AppPausedCallback, this);
	m_xAppPanel.StateChanged.AddListener<Level>(this, &Level::ButtonPanelStateChangedEventHandler);
}

Level::~Level() {
	// detach event handlers
	m_xAppPanel.StateChanged.RemoveListener<Level>(this, &Level::ButtonPanelStateChangedEventHandler);
	s3eDeviceUnRegister(S3E_DEVICE_PAUSE, AppPausedCallback);

	SoundEngine::StopMusicFile();
}

void Level::Initialize() {
	m_xAppPanel.Initialize();
	m_xAppPanel.GetMainButton().SetTexture(FactoryManager::GetTextureFactory().Create("button_toggle_hud"));

	m_xStatsPanel.Initialize();

	SoundEngine::PlayMusicFileLoop(Configuration::GetInstance().LevelSong);

	CreateStar();
}

GameFoundation& Level::GetGameFoundation() {
	return m_xGame;
}

void Level::CreateStar() {
	IW_CALLSTACK_SELF;
	Star* star = dynamic_cast<Star*>(FactoryManager::GetBodyFactory().Create("star"));
	IwAssertMsg(MYAPP, star, ("Could not add star to level. It seems to be of a wrong type. Check its body definition."));
	if (star) {
		star->SetPosition(CIwFVec2(2.0f, 2.0f), 0.0f);
		Add(star);
//		IwAssertMsg(MYAPP, star->CanDrag(), ("Stars must be draggable. Cannot create star..."));
//		star->StartDragging(star->GetPosition());
	}
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

bool Level::GetCompletionInfo(GameFoundation::CompletionInfo& info) {
	if (Page::GetCompletionState() == eCompleted) {
		info = m_xGame.GetCompletionInfo();
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

	// interaction
	m_xInteractor.Update(frame);
	
	// scene and widgets
	m_xCamera.Update(frame.GetScreensize(), frame.GetSimulatedDurationMs());
	m_xBackground.Update(frame);

	// progress indicator
	float progress = m_xGame.GetCompletionDegree();
	m_xStatsPanel.SetProgress(progress);
	m_xStatsPanel.Update(frame);
}

void Level::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.SetViewport(m_xCamera.GetViewport());
	m_xBackground.Render(renderer, frame);
	m_xGame.Render(renderer, frame);

	m_xStatsPanel.Render(renderer, frame);
	m_xAppPanel.Render(renderer, frame);
	
	m_xInteractor.Render(renderer, frame);
}

void Level::OnDoLayout(const CIwSVec2& screensize) {
	// set up the optics according to level definition
	float maxvisibleworldsize = (float)Configuration::GetInstance().MaxVisibleWorldSize;
	m_xCamera.SetGeometry(m_xWorldSize, screensize, maxvisibleworldsize);
	m_xCamera.SetWorldFocus(m_xWorldSize / 2.0f);
	m_xCamera.ZoomOut();

	// create background image to fit the level
	float bgmargin = (float) Configuration::GetInstance().WorldMargin;
	m_xBackground.SetGeometry(m_xWorldSize, screensize, bgmargin);

	// app panel
	uint32 btnsize = 60;
	uint32 btnmargin = 15;
	m_xAppPanel.GetMainButton().SetPosition(
		CIwRect(screensize.x - (btnsize + btnmargin), btnmargin, btnsize, btnsize));
}

CIwFVec2 Level::CalculateRelativeSoundPosition(const CIwFVec2& worldpos) {
	CIwSVec2 soundpixelpos = m_xCamera.GetViewport().WorldToView(worldpos);
	CIwSVec2 centeroffset(IwGxGetScreenWidth() / 2, IwGxGetScreenHeight() / 2);
	soundpixelpos -= centeroffset;
	return CIwFVec2(soundpixelpos.x / (float)centeroffset.x, soundpixelpos.y / (float)centeroffset.y);
}

void Level::ButtonPanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args) {
	m_bIsPaused = !args.IsOpen;
}

int32 Level::AppPausedCallback(void* systemData, void* userData) {
	IW_CALLSTACK_SELF;

	Level* lvl = (Level*) userData;
	lvl->SetPaused();
	return 0;
}
