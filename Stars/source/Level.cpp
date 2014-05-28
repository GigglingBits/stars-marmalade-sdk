#include "Level.h"

#include <string>
#include <strstream>

#include "Debug.h"
#include "Configuration.h"
#include "Nugget.h"
#include "InputManager.h"
#include "FactoryManager.h"
#include "SoundEngine.h"

Level::Level(const CIwFVec2& worldsize, float dustrequirement) :
	Page("level.group", Configuration::GetInstance().LevelSong),
	m_xWorldSize(worldsize),
	m_xGame(dustrequirement, worldsize),
	m_xInteractor(m_xCamera, m_xGame),
	m_xHud(m_xGame),
	m_xPausePanel(eButtonCommandIdToggleHud, s3eKeyAbsGameA),
	m_bIsPaused(false),
	m_bIsSetteling(false),
	m_xBackgroundClouds(m_xGame),
	m_xBannerRect(0, 0, 0, 0) {

	// attach event handlers
	s3eDeviceRegister(S3E_DEVICE_PAUSE, AppPausedCallback, this);
	m_xInteractor.BeginDrawPath.AddListener(this, &Level::BeginDrawPathEventHandler);
	m_xInteractor.EndDrawPath.AddListener(this, &Level::EndDrawPathHandler);
	m_xPausePanel.StateChanged.AddListener<Level>(this, &Level::PausePanelStateChangedEventHandler);
	m_xGame.QuakeImpact.AddListener<Level>(this, &Level::QuakeImpactEventHandler);
	m_xGame.SpriteRemoved.AddListener<Level>(this, &Level::SpriteRemovedEventHandler);
	m_xEventTimer.Elapsed.AddListener(this, &Level::EventTimerEventHandler);
	m_xEventTimer.LastEventFired.AddListener(this, &Level::EventTimerClearedEventHandler);
	
	m_xInteractor.Disable();
	
	// configure the start
	EventArgs args;
	args.eventId = eEventIdShowBanner;
	args.bannerText = "Ready?";
	m_xEventTimer.Enqueue(LEVEL_START_BANNER_LEADIN, args);
		
	args.eventId = eEventIdShowBanner;
	args.bannerText = "";
	m_xEventTimer.Enqueue(LEVEL_START_BANNER_DURATION, args);
	
	args.eventId = eEventIdEnableUserInput;
	m_xEventTimer.Enqueue(0, args);
		
	args.eventId = eEventIdShowBanner;
	args.bannerText = "Go!";
	m_xEventTimer.Enqueue(LEVEL_START_BANNER_LEADIN, args);
		
	args.eventId = eEventIdHideBanner;
	args.bannerText = "";
	m_xEventTimer.Enqueue(LEVEL_START_BANNER_DURATION, args);
}

Level::~Level() {
	// detach event handlers
	m_xEventTimer.LastEventFired.RemoveListener(this, &Level::EventTimerClearedEventHandler);
	m_xEventTimer.Elapsed.RemoveListener(this, &Level::EventTimerEventHandler);
	m_xGame.SpriteRemoved.RemoveListener<Level>(this, &Level::SpriteRemovedEventHandler);
	m_xGame.QuakeImpact.RemoveListener<Level>(this, &Level::QuakeImpactEventHandler);
	m_xPausePanel.StateChanged.RemoveListener<Level>(this, &Level::PausePanelStateChangedEventHandler);
	m_xInteractor.EndDrawPath.RemoveListener(this, &Level::EndDrawPathHandler);
	m_xInteractor.BeginDrawPath.RemoveListener(this, &Level::BeginDrawPathEventHandler);
	s3eDeviceUnRegister(S3E_DEVICE_PAUSE, AppPausedCallback);
}

void Level::Initialize() {
	m_xGame.Initialize();
	
	m_xPausePanel.Initialize();
	m_xPausePanel.GetMainButton().SetTexture(FactoryManager::GetTextureFactory().Create("button_toggle_hud"));

	m_xHud.Initialize();
	m_xBackgroundStars.Initialize();
	
	CreateStar();
	
	// schedule the finish og the level
	// (assuming that it has been populated by the time this is called)
	EventArgs args;
	args.eventId = eEventIdSettle;
	m_xEventTimer.Enqueue(0, args);

	args.eventId = eEventIdFinish;
	m_xEventTimer.Enqueue(LEVEL_COMPLETION_DELAY, args);

	args.eventId = eEventIdDisableUserInput;
	m_xEventTimer.Enqueue(0, args);
	
	args.eventId = eEventIdNoOp;
	m_xEventTimer.Enqueue(LEVEL_LEADOUT_TIME, args);
}

const std::string& Level::GetResourceGroupName() {
	static std::string s("level.group");
	return s;
}

void Level::CreateStar() {
	IW_CALLSTACK_SELF;
	Star* star = dynamic_cast<Star*>(FactoryManager::GetBodyFactory().Create("star"));
	IwAssertMsg(MYAPP, star, ("Could not add star to level. It seems to be of a wrong type. Check its body definition."));
	if (star) {
		star->SetPosition(GetStarHidePosition(), 0.0f);
		Add(star);
		IwAssertMsg(MYAPP, star->CanDrag(), ("Stars must be draggable. Cannot create star..."));
		star->BeginDragging(GetStarHidePosition());
		star->SetAnchorLine(GetStarHidePosition().x);
	}
}

void Level::Add(Body* body) {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, body, ("Could not add body to level. Seems to be NULL..."));
	if (body) {
		m_xGame.Add(body);
	}
}

void Level::Add(uint16 delay, const std::string& body, float ypos, float speed) {
	IW_CALLSTACK_SELF;
	
	if (delay <= 0 && body.empty()) {
		return;
	}
	
	EventArgs args;
	args.eventId = body.empty() ? eEventIdNoOp : eEventIdCreateBody;
	args.bodyName = body;
	args.position.x = m_xWorldSize.x * 1.5f;
	args.position.y = ypos;
	args.speed.y = 0.0f;
	args.speed.x = -speed;
	
	m_xEventTimer.Enqueue(delay, args);
}

void Level::CreateBody(const std::string& bodyName, const CIwFVec2 pos, const CIwFVec2 speed) {
	IW_CALLSTACK_SELF;
	
	if (bodyName.empty()) {
		return;
	}

	BodyFactory& factory = FactoryManager::GetBodyFactory();
	if (Body* body = factory.Create(bodyName)) {
		body->SetPosition(pos);
		body->SetSpeed(speed);
		Add(body);
	} else {
		IwAssertMsg(MYAPP, body, ("Failed to create new body with name '%s'", bodyName.c_str()));
	}
}

void Level::SetPaused(bool paused) {
	IwTrace(MYAPP, (paused ? "Pause" : "Unpause"));
	if (paused) {
		m_xPausePanel.OpenPanel();
		m_xHud.SetEnabled(false);
		HideBannerText();
	} else {
		m_xHud.SetEnabled(true);
	}
	m_bIsPaused = paused;
}

bool Level::IsPaused() {
	return m_bIsPaused;
}

void Level::HideStar() {
	IwTrace(MYAPP, ("Hiding star"));
	SetStarAnchor(GetStarHidePosition());
}

void Level::ShowStar() {
	IwTrace(MYAPP, ("Showing star"));
	SetStarAnchor(GetStarIdlePosition());
}

void Level::SetStarAnchor(const CIwFVec2& pos) {
	if (Star* star = m_xGame.GetStar()) {
		// adjust retraction line
		star->SetAnchorLine(pos.x);
		// move fast
		star->FollowPath(1, &pos, (float)Configuration::GetInstance().PathSpeed);
	}
}

void Level::SetStarPath(int samplecount, const CIwFVec2* samplepoints) {
	IW_CALLSTACK_SELF;
	if (Star* star = m_xGame.GetStar()) {
		if (samplecount > 0) {
			IwAssertMsg(MYAPP, star->IsDragging(), ("Star is not being dragged. Something's wrong!"));
			star->FollowPath(samplecount, samplepoints, (float)Configuration::GetInstance().PathSpeed);
			IwTrace(MYAPP, ("Setting %i points path to star", samplecount));
			m_xCompletionInfo.IncrementPathsStarted();
		}
	}
}

const LevelCompletionInfo& Level::GetCompletionInfo() {
	return m_xCompletionInfo;
}

void Level::StartSection(const std::string& bannertext) {
	IW_CALLSTACK_SELF;

	if (!bannertext.empty()) {
		EventArgs args;
		args.eventId = eEventIdShowBanner;
		args.bannerText = bannertext;
		m_xEventTimer.Enqueue(LEVEL_SECTION_BANNER_LEADIN, args);
		
		args.eventId = eEventIdShowBanner;
		args.bannerText = "";
		m_xEventTimer.Enqueue(LEVEL_SECTION_BANNER_DURATION, args);
		
		args.eventId = eEventIdSettle;
		m_xEventTimer.Enqueue(LEVEL_SECTION_BANNER_LEADOUT, args);
	}
}

void Level::EndSection() {
	; // nothing happens here; the next session will start with settling
}

CIwFVec2 Level::GetStarIdlePosition() {
	return CIwFVec2(0.0f, m_xWorldSize.y / 2.0f);
}

CIwFVec2 Level::GetStarHidePosition() {
	const float offset = 4.0f;	
	CIwFVec2 pos(-offset, m_xWorldSize.y / 2.0f);
	if (m_xCompletionInfo.IsCleared()) {
		pos.x = m_xWorldSize.x + offset;
	}
	return pos;
}

void Level::ShowBannerText(const std::string& text) {
	IwTrace(MYAPP, ("Showing banner: %s", text.c_str()));
	m_sBannerText = text;
}

void Level::HideBannerText() {
	IwTrace(MYAPP, ("Hiding banner"));
	m_sBannerText.clear();
}

void Level::ShowStatsBanner() {
	ShowBannerText(m_xCompletionInfo.IsCleared() ? "Well done!" : "Close. But no cigar...");
	if (m_xCompletionInfo.IsCleared()) {
		SoundEngine::GetInstance().PlaySoundEffect("level_win");
	}
}

CIwFVec2 Level::CalculateRelativeSoundPosition(const CIwFVec2& worldpos) {
	CIwSVec2 soundpixelpos = m_xCamera.GetViewport().WorldToView(worldpos);
	CIwSVec2 centeroffset(IwGxGetScreenWidth() / 2, IwGxGetScreenHeight() / 2);
	soundpixelpos -= centeroffset;
	return CIwFVec2(soundpixelpos.x / (float)centeroffset.x, soundpixelpos.y / (float)centeroffset.y);
}

void Level::Conclude() {
	m_xCompletionInfo.SetDustFillAmount(m_xGame.GetDustFillAmount());
	m_xCompletionInfo.SetDustFillMax(m_xGame.GetDustFillMax());
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
	uint32 btnsize = extents / 8;
	uint32 btnmargin = extents / 30;
	m_xPausePanel.GetMainButton().SetPosition(
		CIwRect(screensize.x - (btnsize + btnmargin),
				btnmargin, btnsize, btnsize));
	
	// banner location
	int bannerheight = extents / 7;
	m_xBannerRect.Make(
		0, (screensize.y - bannerheight) / 2,
		screensize.x, bannerheight);
}

void Level::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xPausePanel.Update(frame);

	if (IsPaused()) {
		return;
	}

	// manage level progress (prevent creation of new bodies when level is setteling)
	if (m_bIsSetteling) {
		m_bIsSetteling = m_xGame.IsGameGoing();
	}
	if (!m_bIsSetteling) {
		m_xEventTimer.Update(frame.GetSimulatedDurationMs());
	}
	
	// update game logic (create new, remove dead, etc...)
	m_xGame.Update(frame);

	// interaction
	m_xInteractor.Update(frame);
	
	// scene and widgets
	m_xCamera.Update(frame.GetScreensize(), frame.GetSimulatedDurationMs());
	m_xBackgroundStars.Update(frame);
	m_xBackgroundClouds.Update(frame.GetAvgSimulatedDurationMs());
	
	m_xHud.Update(frame);
}

void Level::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.SetViewport(m_xCamera.GetViewport());
	m_xBackgroundStars.Render(renderer, frame);
	m_xGame.Render(renderer, frame);

	m_xPausePanel.Render(renderer, frame);
	m_xInteractor.Render(renderer, frame);

	m_xHud.Render(renderer, frame);
	
	if (!m_sBannerText.empty()) {
		renderer.DrawText(m_sBannerText, m_xBannerRect, Renderer::eFontTypeLarge, 0xffccfaff);
	}
}

int32 Level::AppPausedCallback(void* systemData, void* userData) {
	IW_CALLSTACK_SELF;

	Level* lvl = (Level*) userData;
	lvl->SetPaused(true);
	return 0;
}

void Level::PausePanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args) {
	SetPaused(args.IsPanelOpen);
}

void Level::QuakeImpactEventHandler(const GameFoundation& sender, const GameFoundation::QuakeImpactArgs& args) {
	m_xCamera.StartQuakeEffect(args.amplitude, 700);
}

void Level::SpriteRemovedEventHandler(const GameFoundation& sender, const GameFoundation::SpriteRemovedArgs& args) {
	if (args.sprite->GetTypeName() == Nugget::TypeName()) {
		if (args.outofbounds) {
			m_xCompletionInfo.IncrementNuggetsMissed();
		} else {
			m_xCompletionInfo.IncrementNuggetsCollected();
		}
	}
}

void Level::EventTimerEventHandler(const MulticastEventTimer<EventArgs>& sender, const EventArgs& args) {
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
			ShowStar();
			break;
		}
		case eEventIdDisableUserInput: {
			m_xInteractor.Disable();
			HideStar();
			break;
		}
		case eEventIdSettle: {
			m_bIsSetteling = true;
			break;
		}
		case eEventIdCreateBody: {
			CreateBody(args.bodyName, args.position, args.speed);
			break;
		}
		case eEventIdFinish: {
			Conclude();
			ShowStatsBanner();
			break;
		}
	}
}

void Level::EventTimerClearedEventHandler(const MulticastEventTimer<EventArgs>& sender, const int& dummy) {
	SetCompletionState(eCompleted);
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
	SetStarPath(path.count, path.samplepos);
}
