#include "Level.h"

#include <string>
#include <strstream>

#include "Debug.h"
#include "Configuration.h"
#include "InputManager.h"
#include "FactoryManager.h"

Level::Level(const CIwFVec2& worldsize, float dustrequirement) :
	Page("level.group", Configuration::GetInstance().LevelSong),
	m_xWorldSize(worldsize),
	m_xGame(dustrequirement, worldsize),
	m_xInteractor(m_xCamera, m_xGame),
	m_xHud(m_xGame),
	m_xAppPanel(eButtonCommandIdToggleHud, s3eKeyAbsGameA),
	m_bIsPaused(false),
	m_xBackgroundClouds(m_xGame) {

	// attach event handlers
	s3eDeviceRegister(S3E_DEVICE_PAUSE, AppPausedCallback, this);
	m_xInteractor.BeginDrawPath.AddListener(this, &Level::BeginDrawPathEventHandler);
	m_xInteractor.EndDrawPath.AddListener(this, &Level::EndDrawPathHandler);
	m_xAppPanel.StateChanged.AddListener<Level>(this, &Level::AppPanelStateChangedEventHandler);
	m_xGame.QuakeImpact.AddListener<Level>(this, &Level::QuakeImpactEventHandler);
	m_xEventTimer.Elapsed.AddListener(this, &Level::EventTimerEventHandler);
	m_xEventTimer.LastEventFired.AddListener(this, &Level::EventTimerClearedEventHandler);
		
	// configure the start
	EventArgs args;
	args.eventId = eEventIdDisableUserInput;
	m_xEventTimer.Enqueue(0, args);
		
	args.eventId = eEventIdShowBanner;
	args.bannerText = "3";
	m_xEventTimer.Enqueue(200, args);
		
	args.eventId = eEventIdHideBanner;
	args.bannerText = "";
	m_xEventTimer.Enqueue(1100, args);

	args.eventId = eEventIdShowBanner;
	args.bannerText = "2";
	m_xEventTimer.Enqueue(200, args);
		
	args.eventId = eEventIdHideBanner;
	args.bannerText = "";
	m_xEventTimer.Enqueue(1100, args);
		
	args.eventId = eEventIdShowBanner;
	args.bannerText = "1";
	m_xEventTimer.Enqueue(200, args);
		
	args.eventId = eEventIdHideBanner;
	args.bannerText = "";
	m_xEventTimer.Enqueue(1100, args);

	args.eventId = eEventIdShowBanner;
	args.bannerText = "Go!";
	m_xEventTimer.Enqueue(200, args);
		
	args.eventId = eEventIdHideBanner;
	args.bannerText = "";
	m_xEventTimer.Enqueue(1100, args);

	args.eventId = eEventIdEnableUserInput;
	m_xEventTimer.Enqueue(0, args);
}

Level::~Level() {
	// detach event handlers
	m_xEventTimer.LastEventFired.RemoveListener(this, &Level::EventTimerClearedEventHandler);
	m_xEventTimer.Elapsed.RemoveListener(this, &Level::EventTimerEventHandler);
	m_xGame.QuakeImpact.RemoveListener<Level>(this, &Level::QuakeImpactEventHandler);
	m_xAppPanel.StateChanged.RemoveListener<Level>(this, &Level::AppPanelStateChangedEventHandler);
	m_xInteractor.EndDrawPath.RemoveListener(this, &Level::EndDrawPathHandler);
	m_xInteractor.BeginDrawPath.RemoveListener(this, &Level::BeginDrawPathEventHandler);
	s3eDeviceUnRegister(S3E_DEVICE_PAUSE, AppPausedCallback);
}

void Level::Initialize() {
	m_xAppPanel.Initialize();
	m_xAppPanel.GetMainButton().SetTexture(FactoryManager::GetTextureFactory().Create("button_toggle_hud"));
	
	m_xHud.Initialize();
	m_xBackgroundStars.Initialize();
	
	CreateStar();

	EventArgs args;
	args.eventId = eEventIdFinish;
	m_xEventTimer.Enqueue(LEVEL_COMPLETION_DELAY, args);
}

const std::string& Level::GetResourceGroupName() {
	static std::string s("sprites/sprites.group");
	return s;
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

void Level::Add(uint16 delay, const std::string& body, float ypos) {
	IW_CALLSTACK_SELF;
	
	EventArgs args;
	args.eventId = eEventIdCreateBody;
	args.bodyName = body;
	args.position.x = m_xWorldSize.x * 1.5f;
	args.position.y = ypos;
	
	m_xEventTimer.Enqueue(delay, args);
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

const Level::CompletionInfo& Level::GetCompletionInfo() {
	return m_xCompletionInfo;
}

float Level::GetCompletionDegree() {
	uint32 total = m_xEventTimer.GetTotalDuration();
	return total == 0 ? 1.0f : std::min<float>(1.0f, (float)m_xEventTimer.GetElapsedTime() / (float)total);
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

void Level::EnableUserInput() {
	
}

void Level::DisableUserInput() {
	
}

void Level::ShowBannerText(const std::string& text) {
	m_sBannerText = text;
}

void Level::HideBannerText() {
	m_sBannerText = "";
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
	m_xBackgroundClouds.SetGeometry(m_xWorldSize, screensize, bgmargin);
	
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

	// update game logic (create new, remove dead, etc...)
	m_xEventTimer.Update(frame.GetSimulatedDurationMs());
	m_xGame.Update(frame);

	// interaction
	m_xInteractor.Update(frame);
	
	// scene and widgets
	m_xCamera.Update(frame.GetScreensize(), frame.GetSimulatedDurationMs());
	m_xBackgroundStars.Update(frame);
	m_xBackgroundClouds.Update(frame);
	
	m_xHud.SetLevelProgress(GetCompletionDegree());
	m_xHud.Update(frame);
}

void Level::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.SetViewport(m_xCamera.GetViewport());
	m_xBackgroundStars.Render(renderer, frame);
	m_xBackgroundClouds.Render(renderer, frame);
	m_xGame.Render(renderer, frame);

	m_xAppPanel.Render(renderer, frame);	
	m_xInteractor.Render(renderer, frame);

	m_xHud.Render(renderer, frame);
	
	if (!m_sBannerText.empty()) {
		renderer.DrawText(m_sBannerText, m_xWorldSize / 2.0f, Renderer::eFontTypeLarge, 0xffccfaff);
	}
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

void Level::EventTimerEventHandler(const EventTimer<EventArgs>& sender, const EventArgs& args) {
	IW_CALLSTACK_SELF;
	
	switch (args.eventId) {
		case eEventIdNoOp: {
			break;
		}
		case eEventIdShowBanner: {
			ShowBannerText(args.bannerText);
			break;
		}
		case eEventIdHideBanner: {
			HideBannerText();
			break;
		}
		case eEventIdEnableUserInput: {
			m_xInteractor.Enable();
			break;
		}
		case eEventIdDisableUserInput: {
			m_xInteractor.Disable();
			break;
		}
		case eEventIdSuspendEventTimer: {
			break;
		}
		case eEventIdCreateBody: {
			CreateBody(args.bodyName, args.position,
					   CIwFVec2(-Configuration::GetInstance().ObjectSpeed, 0.0f));
			break;
		}
		case eEventIdFinish: {
			SetCompletionState(eCompleted);
			break;
		}
	}
}

void Level::CreateBody(const std::string& bodyName, const CIwFVec2 pos, const CIwFVec2 speed) {
	IW_CALLSTACK_SELF;

	BodyFactory& factory = FactoryManager::GetBodyFactory();
	if (Body* body = factory.Create(bodyName)) {
		body->SetPosition(pos);
		body->SetSpeed(speed);
		Add(body);
	} else {
		IwAssertMsg(MYAPP, body, ("Failed to create new body with name '%s'", bodyName.c_str()));
	}
}

void Level::EventTimerClearedEventHandler(const EventTimer<EventArgs>& sender, const int& dummy) {
	m_xCompletionInfo.IsCleared = true;
	m_xCompletionInfo.DustFillPercent = m_xGame.GetDustFillPercent();
}
