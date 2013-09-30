#include "Level.h"

#include <string>
#include <strstream>

#include "Debug.h"
#include "Configuration.h"
#include "InputManager.h"
#include "SoundEngine.h"
#include "FactoryManager.h"

Level::Level(const CIwFVec2& worldsize, float dustrequirement, std::string background) :
	m_xWorldSize(worldsize),
	m_xGame(dustrequirement, worldsize),
	m_xBackground(background, m_xGame), 
	m_xInteractor(m_xCamera, m_xGame),
	m_xHud(m_xGame),
	m_iCompletionTimer(0),
	m_xAppPanel(eButtonCommandIdToggleHud, s3eKeyP),
	m_bIsPaused(false){

	// attach event handlers
	s3eDeviceRegister(S3E_DEVICE_PAUSE, AppPausedCallback, this);
	m_xInteractor.BeginDrawPath.AddListener(this, &Level::BeginDrawPathEventHandler);
	m_xInteractor.EndDrawPath.AddListener(this, &Level::EndDrawPathHandler);
	m_xAppPanel.StateChanged.AddListener<Level>(this, &Level::AppPanelStateChangedEventHandler);
	m_xGame.QuakeImpact.AddListener<Level>(this, &Level::QuakeImpactEventHandler);
}

Level::~Level() {
	// detach event handlers
	m_xGame.QuakeImpact.RemoveListener<Level>(this, &Level::QuakeImpactEventHandler);
	m_xAppPanel.StateChanged.RemoveListener<Level>(this, &Level::AppPanelStateChangedEventHandler);
	m_xInteractor.EndDrawPath.RemoveListener(this, &Level::EndDrawPathHandler);
	m_xInteractor.BeginDrawPath.RemoveListener(this, &Level::BeginDrawPathEventHandler);
	s3eDeviceUnRegister(S3E_DEVICE_PAUSE, AppPausedCallback);

	SoundEngine::StopMusicFile();
}

void Level::Initialize() {
	m_xAppPanel.Initialize();
	m_xAppPanel.GetMainButton().SetTexture(FactoryManager::GetTextureFactory().Create("button_toggle_hud"));
	
	m_xHud.Initialize();

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
		star->SetPosition(GetStarStartPosition(), 0.0f);
		Add(star);
		IwAssertMsg(MYAPP, star->CanDrag(), ("Stars must be draggable. Cannot create star..."));
		star->BeginDragging(GetStarStartPosition());
		star->SetAnchorLine(GetStarStartPosition().x);
	}
}

void Level::Add(Body* body) {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, body, ("Could not add body to level. Seems to be NULL..."));
	if (body) {
		m_xGame.Add(body);
	}
}

void Level::SetPaused(bool paused) {
	if (paused) {
		m_xAppPanel.OpenPanel();
		m_xHud.SetEnabled(false);
	} else {
		m_xHud.SetEnabled(true);
	}
	m_bIsPaused = paused;
}

bool Level::IsPaused() {
	return m_bIsPaused;
}

bool Level::GetCompletionInfo(GameFoundation::CompletionInfo& info) {
	if (GetCompletionState() == eCompleted) {
		info = m_xGame.GetCompletionInfo();
		return true;
	}
	return false;
}

float Level::GetStarMoveForce() {
	return 10.0f;
}

float Level::GetStarRestForce() {
	return 10.0f;	
}

CIwFVec2 Level::GetStarStartPosition() {
	return CIwFVec2(0.0f, m_xWorldSize.y / 2.0f);
}

void Level::BeginDrawPathEventHandler(const LevelInteractor& sender, const CIwFVec2& pos) {
	IW_CALLSTACK_SELF;
	if (Star* star = m_xGame.GetStar()) {
		IwAssertMsg(MYAPP, star->IsDragging(), ("Star is not being dragged. Something's wrong!"));
		star->MoveDragging(star->GetPosition());
	}
}

void Level::EndDrawPathHandler(const LevelInteractor& sender, const LevelInteractor::PathEventArgs& path) {
	IW_CALLSTACK_SELF;
	if (Star* star = m_xGame.GetStar()) {
		if (path.count > 0) {
			IwAssertMsg(MYAPP, star->IsDragging(), ("Star is not being dragged. Something's wrong!"));
			star->FollowPath(path.count, path.samplepos, (float)Configuration::GetInstance().PathSpeed);
		}
	}
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
	int extents = GetScreenExtents();
	uint32 btnsize = extents / 10;
	uint32 btnmargin = extents / 30;
	m_xAppPanel.GetMainButton().SetPosition(
		CIwRect(screensize.x - (btnsize + btnmargin),
				btnmargin, btnsize, btnsize));
}

void Level::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xAppPanel.Update(frame);

	if (IsPaused()) {
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
	m_xHud.Update(frame);
}

void Level::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.SetViewport(m_xCamera.GetViewport());
	m_xBackground.Render(renderer, frame);
	m_xGame.Render(renderer, frame);

	m_xAppPanel.Render(renderer, frame);	
	m_xInteractor.Render(renderer, frame);

	m_xHud.Render(renderer, frame);
}

CIwFVec2 Level::CalculateRelativeSoundPosition(const CIwFVec2& worldpos) {
	CIwSVec2 soundpixelpos = m_xCamera.GetViewport().WorldToView(worldpos);
	CIwSVec2 centeroffset(IwGxGetScreenWidth() / 2, IwGxGetScreenHeight() / 2);
	soundpixelpos -= centeroffset;
	return CIwFVec2(soundpixelpos.x / (float)centeroffset.x, soundpixelpos.y / (float)centeroffset.y);
}

int32 Level::AppPausedCallback(void* systemData, void* userData) {
	IW_CALLSTACK_SELF;

	Level* lvl = (Level*) userData;
	lvl->SetPaused(true);
	return 0;
}

void Level::AppPanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args) {
	SetPaused(args.IsPanelOpen);
}

void Level::QuakeImpactEventHandler(const GameFoundation& sender, const GameFoundation::QuakeImpactArgs& args) {
	m_xCamera.StartQuakeEffect(args.amplitude, 700);
}
