#include "Level.h"

#include <string>
#include <strstream>

#include "Debug.h"
#include "Configuration.h"
#include "Nugget.h"
#include "InputManager.h"
#include "FactoryManager.h"
#include "Enemy.h"
#include "Bird.h"
#include "Buff.h"
#include "Nugget.h"

Level::Level(const CIwFVec2& worldsize, float dustrequirement, uint8 lives) :
	Page("level.group", Configuration::GetInstance().LevelSong),
	m_xWorldSize(worldsize),
	m_xGame(dustrequirement, worldsize),
	m_xInteractor(m_xCamera, m_xGame),
	m_xPausePanel(eButtonCommandIdToggleHud, s3eKeyAbsGameA),
	m_xBackButton(eButtonCommandIdNone, s3eKeyAbsGameD),
	m_bIsPaused(false),
	m_bIsSetteling(false),
	m_bIsAborting(false),
	m_uiStarLiveCount(lives),
	m_iStarReviveDelay(-1),
	m_xBackgroundClouds(m_xGame),
	m_xBannerRect(0, 0, 0, 0) {

	// attach event handlers
	s3eDeviceRegister(S3E_DEVICE_PAUSE, AppPausedCallback, this);
	m_xInteractor.PathChanged.AddListener(this, &Level::PathChangedEventHandler);
	m_xPausePanel.StateChanged.AddListener<Level>(this, &Level::PausePanelStateChangedEventHandler);
	m_xGame.QuakeImpact.AddListener<Level>(this, &Level::QuakeImpactEventHandler);
	m_xGame.SpriteAdded.AddListener<Level>(this, &Level::SpriteAddedEventHandler);
	m_xGame.SpriteRemoved.AddListener<Level>(this, &Level::SpriteRemovedEventHandler);
	m_xGame.BuffCollected.AddListener(this, &Level::BuffCollectedHandler);
	m_xHud.GetBuffPanel().BuffTrigger.AddListener(this, &Level::BuffTriggerHandler);
	m_xEventTimer.Elapsed.AddListener(this, &Level::EventTimerEventHandler);
	m_xEventTimer.LastEventFired.AddListener(this, &Level::EventTimerClearedEventHandler);
		m_xBackButton.PressedEvent.AddListener(this, &Level::ButtonReleasedEventHandler);
		
	m_xInteractor.Disable();

	ScheduleBegin();
}

Level::~Level() {
	// detach event handlers
	m_xBackButton.PressedEvent.RemoveListener(this, &Level::ButtonReleasedEventHandler);
	m_xEventTimer.LastEventFired.RemoveListener(this, &Level::EventTimerClearedEventHandler);
	m_xEventTimer.Elapsed.RemoveListener(this, &Level::EventTimerEventHandler);
	m_xHud.GetBuffPanel().BuffTrigger.RemoveListener(this, &Level::BuffTriggerHandler);
	m_xGame.BuffCollected.RemoveListener(this, &Level::BuffCollectedHandler);
	m_xGame.SpriteRemoved.RemoveListener<Level>(this, &Level::SpriteRemovedEventHandler);
	m_xGame.SpriteAdded.RemoveListener<Level>(this, &Level::SpriteAddedEventHandler);
	m_xGame.QuakeImpact.RemoveListener<Level>(this, &Level::QuakeImpactEventHandler);
	m_xPausePanel.StateChanged.RemoveListener<Level>(this, &Level::PausePanelStateChangedEventHandler);
	m_xInteractor.PathChanged.RemoveListener(this, &Level::PathChangedEventHandler);
	s3eDeviceUnRegister(S3E_DEVICE_PAUSE, AppPausedCallback);
}

void Level::Initialize() {
	m_xGame.Initialize();
	
	m_xPausePanel.Initialize();
	m_xPausePanel.GetMainButton().SetTexture(FactoryManager::GetTextureFactory().Create("button_toggle_hud"));

	m_xBackgroundStars.Initialize();

	m_xPath.InitializeTexture();
	m_xHud.Initialize();
	m_xHud.GetLives().SetNumber(m_uiStarLiveCount);
	m_xHud.ShowLives(m_uiStarLiveCount > 1);
	
	m_xCompletionInfo.SetNumberOfLifesGranted(m_uiStarLiveCount);
	
	CreateStar();
	
	ScheduleFinish();
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
		star->SetId("star");
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

void Level::Add(uint16 delay, const std::string& body, float ypos, float speed, const Body::BuffProbabilities& probs) {
	IW_CALLSTACK_SELF;
	
	if (delay <= 0 && body.empty()) {
		return;
	}
	
	TimerEventArgs args;
	args.eventId = body.empty() ? eTimerEventIdNoOp : eTimerEventIdCreateBody;
	args.bodyName = body;
	args.position.x = m_xWorldSize.x * 1.5f;
	args.position.y = ypos;
	args.speed.y = 0.0f;
	args.speed.x = -speed;
	args.probs = probs;
	
	m_xEventTimer.Enqueue(delay, args);
}

void Level::CreateBody(const std::string& bodyName, const CIwFVec2 pos, const CIwFVec2 speed, const Body::BuffProbabilities& probs) {
	IW_CALLSTACK_SELF;
	
	if (bodyName.empty()) {
		return;
	}

	BodyFactory& factory = FactoryManager::GetBodyFactory();
	if (Body* body = factory.Create(bodyName)) {
		body->SetPosition(pos);
		body->SetSpeed(speed);
		body->SetBuffProbabilities(probs);
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

bool Level::ReviveStar(uint16 frametime) {
	if (m_iStarReviveDelay >= 0) {
		// count down
		m_iStarReviveDelay -= frametime;
		if (m_iStarReviveDelay < 0 && !m_xGame.GetStar()) {
			// create star when 0 is reached
			CreateStar();
			ShowStar();
			m_iStarReviveDelay = -1;
		}
	} else if (!m_xGame.GetStar()) {
		// star not present
		// must just have died;
		// start the countdown
		long lives = m_xHud.GetLives().GetNumber() - 1;
		m_xHud.GetLives().SetNumber(lives);
		if (lives > 0) {
			// got more lives... start count-down
			m_iStarReviveDelay = Configuration::GetInstance().StarBirthDelay;
		} else {
			// no lives left... refuse to revive
			return false;
		}
	}
	return true;
}

void Level::AbortLevel() {
	m_xGame.ClearDust();
	m_xEventTimer.Clear(true);
	ScheduleFinish(false);
	m_bIsAborting = true;
}

bool Level::IsAborting() {
	return m_bIsAborting;
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
		star->SetAnchorLine(pos.x);
		star->Passify();
	}
}

void Level::SetStarPath(const std::vector<CIwFVec2>& path) {
	IW_CALLSTACK_SELF;
	if (Star* star = m_xGame.GetStar()) {
		IwAssertMsg(MYAPP, star->IsDragging(), ("Star is not being dragged. Something's wrong!"));
		star->FollowPath(path);
		m_xCompletionInfo.IncrementPathsStarted();
	}
}

void Level::CancelStarPath() {
	IW_CALLSTACK_SELF;
	if (Star* star = m_xGame.GetStar()) {
		IwAssertMsg(MYAPP, star->IsDragging(), ("Star is not being dragged. Something's wrong!"));
		if (star->IsFollowingPath()) {
			std::vector<CIwFVec2> path;
			path.push_back(star->GetPosition());
			star->FollowPath(path);
		}
	}
}

const LevelCompletionInfo& Level::GetCompletionInfo() {
	return m_xCompletionInfo;
}

void Level::StartSection(const std::string& bannertext) {
	IW_CALLSTACK_SELF;

	if (!bannertext.empty()) {
		TimerEventArgs args;
		args.eventId = eTimerEventIdShowBanner;
		args.bannerText = bannertext;
		m_xEventTimer.Enqueue(LEVEL_SECTION_BANNER_LEADIN, args);
		
		args.eventId = eTimerEventIdShowBanner;
		args.bannerText = "";
		m_xEventTimer.Enqueue(LEVEL_SECTION_BANNER_DURATION, args);
		
		args.eventId = eTimerEventIdSettle;
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
	const float offset = 6.0f;
	
	CIwFVec2 pos(-offset, m_xWorldSize.y / 2.0f);
	if (m_xCompletionInfo.IsLevelAchieved()) {
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

CIwFVec2 Level::CalculateRelativeSoundPosition(const CIwFVec2& worldpos) {
	CIwSVec2 soundpixelpos = m_xCamera.GetViewport().WorldToView(worldpos);
	CIwSVec2 centeroffset(IwGxGetScreenWidth() / 2, IwGxGetScreenHeight() / 2);
	soundpixelpos -= centeroffset;
	return CIwFVec2(soundpixelpos.x / (float)centeroffset.x, soundpixelpos.y / (float)centeroffset.y);
}

void Level::ScheduleBegin() {
	// configure the start
	TimerEventArgs args;
	args.eventId = eTimerEventIdShowBanner;
	args.bannerText = "Ready?";
	m_xEventTimer.Enqueue(LEVEL_START_BANNER_LEADIN, args);
	
	args.eventId = eTimerEventIdShowBanner;
	args.bannerText = "";
	m_xEventTimer.Enqueue(LEVEL_START_BANNER_DURATION, args);
	
	args.eventId = eTimerEventIdEnableUserInput;
	m_xEventTimer.Enqueue(0, args);
	
	args.eventId = eTimerEventIdShowBanner;
	args.bannerText = "Go!";
	m_xEventTimer.Enqueue(LEVEL_START_BANNER_LEADIN, args);
	
	args.eventId = eTimerEventIdHideBanner;
	args.bannerText = "";
	m_xEventTimer.Enqueue(LEVEL_START_BANNER_DURATION, args);
}

void Level::ScheduleFinish(bool settle) {
	// schedule the finish of the level
	// (assuming that it has been populated by the time this is called)
	TimerEventArgs args;
	
	if (settle) {
		args.eventId = eTimerEventIdSettle;
		m_xEventTimer.Enqueue(0, args);
	}
		
	args.eventId = eTimerEventIdFinish;
	m_xEventTimer.Enqueue(LEVEL_COMPLETION_DELAY, args);
	
	args.eventId = eTimerEventIdDisableUserInput;
	m_xEventTimer.Enqueue(0, args);
	
	args.eventId = eTimerEventIdNoOp;
	m_xEventTimer.Enqueue(LEVEL_LEADOUT_TIME, args);
}

void Level::ResetPlayerInteraction() {
	m_xHud.GetBuffPanel().ClearBuffs();
	m_xPath.ClearPath();
	CancelStarPath();
}

void Level::Conclude() {
	m_xCompletionInfo.SetDustFillAmount(m_xGame.GetDustFillAmount());
	m_xCompletionInfo.SetDustFillMax(m_xGame.GetDustFillMax());
	m_xCompletionInfo.SetNumberOfLifesLeft(m_xHud.GetLives().GetNumber());
	m_xCompletionInfo.Evaluate();
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

	// revive star, if it has died
	if (!IsAborting() && !ReviveStar(frame.GetSimulatedDurationMs())) {
		// revive needed, but failed -> game over
		AbortLevel();
	}
	
	// update game logic (create new, remove dead, etc...)
	m_xGame.Update(frame);

	// scene and widgets
	m_xCamera.Update(frame.GetScreensize(), frame.GetSimulatedDurationMs());
	m_xBackgroundStars.Update(frame);
	m_xBackgroundClouds.Update(frame.GetAvgSimulatedDurationMs());
	
	m_xPath.Update(frame);
	m_xHud.GetDustVial().SetDustAmount(m_xGame.GetDustFillPercent(), m_xGame.GetDustQueuedPercent());
	m_xHud.Update(frame);
}

void Level::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.SetViewport(m_xCamera.GetViewport());
	m_xBackgroundStars.Render(renderer, frame);
	m_xGame.Render(renderer, frame);

	m_xPausePanel.Render(renderer, frame);

	m_xPath.Render(renderer, frame);
	m_xHud.Render(renderer, frame);
	
	if (!m_sBannerText.empty()) {
		renderer.DrawText(m_sBannerText, m_xBannerRect, Renderer::eFontTypeLarge, GAME_COLOUR_FONT_MAIN);
	}
}

int32 Level::AppPausedCallback(void* systemData, void* userData) {
	IW_CALLSTACK_SELF;

	Level* lvl = (Level*) userData;
	lvl->SetPaused(true);
	return 0;
}

void Level::ButtonReleasedEventHandler(const InputManager::VirtualButton& sender, const InputManager::VirtualButton::EventArgs& args) {
	if (sender.key == s3eKeyAbsGameD) {
		if (IsPaused()) {
			InputManager::ButtonEventArgs arg;
			arg.cmdid = eButtonCommandIdQuitLevel;
			InputManager& im = InputManager::GetInstance();
			im.ButtonPressedEvent.Invoke(im, arg);
			im.ButtonReleasedEvent.Invoke(im, arg);
		} else {
			SetPaused(true);
		}
		args.handled = true;
	}
}

void Level::PausePanelStateChangedEventHandler(const ButtonPanel& sender, const ButtonPanel::EventArgs& args) {
	SetPaused(args.IsPanelOpen);
}

void Level::QuakeImpactEventHandler(const GameFoundation& sender, const GameFoundation::QuakeImpactArgs& args) {
	m_xCamera.StartQuakeEffect(args.amplitude, 700);
}

void Level::SpriteAddedEventHandler(const GameFoundation& sender, const Sprite& args) {
	if (IsAborting()) {
		return;
	}

	if (dynamic_cast<const Nugget*>(&args)) {
		m_xCompletionInfo.IncrementNuggetsDeployed();
	} else if (dynamic_cast<const Bird*>(&args)) {
		m_xCompletionInfo.IncrementBirdsDeployed();
	} else if (dynamic_cast<const Buff*>(&args)) {
		m_xCompletionInfo.IncrementBuffsDeployed();
	}
}

void Level::SpriteRemovedEventHandler(const GameFoundation& sender, const GameFoundation::SpriteRemovedArgs& args) {
	if (IsAborting()) {
		return;
	}
	
	if (dynamic_cast<const Nugget*>(args.sprite)) {
		if (!args.outofbounds) {
			m_xCompletionInfo.IncrementNuggetsCollected();
		}
	} else if (const Enemy* enemy = dynamic_cast<const Enemy*>(args.sprite)) {
		if (!args.outofbounds) {
			if (dynamic_cast<const Bird*>(enemy)) {
				m_xCompletionInfo.IncrementBirdsKilled();
			}
		}
	} else if (dynamic_cast<const Star*>(args.sprite)) {
		m_xCompletionInfo.IncrementBirdsCollided();
	}
}

void Level::EventTimerEventHandler(const MulticastEventTimer<TimerEventArgs>& sender, const TimerEventArgs& args) {
	IW_CALLSTACK_SELF;
	
	switch (args.eventId) {
		case eTimerEventIdNoOp: {
			break;
		}
		case eTimerEventIdShowBanner: {
			ShowBannerText(args.bannerText);
			break;
		}
		case eTimerEventIdHideBanner: {
			HideBannerText();
			break;
		}
		case eTimerEventIdEnableUserInput: {
			m_xInteractor.Enable();
			ShowStar();
			break;
		}
		case eTimerEventIdDisableUserInput: {
			m_xInteractor.Disable();
			HideStar();
			break;
		}
		case eTimerEventIdSettle: {
			m_bIsSetteling = true;
			break;
		}
		case eTimerEventIdCreateBody: {
			CreateBody(args.bodyName, args.position, args.speed, args.probs);
			break;
		}
		case eTimerEventIdFinish: {
			ResetPlayerInteraction();
			Conclude();
			break;
		}
	}
}

void Level::EventTimerClearedEventHandler(const MulticastEventTimer<TimerEventArgs>& sender, const int& dummy) {
	SetCompletionState(eCompleted);
}

void Level::PathChangedEventHandler(const LevelInteractor& sender, const LevelInteractor::PathEventArgs& path) {
	IW_CALLSTACK_SELF;
	if (path.complete) {
		m_xPath.ClearPath();
		SetStarPath(path.path);
	} else {
		m_xPath.ImportPath(path.path);
	}
}

void Level::BuffCollectedHandler(const GameFoundation& sender, const GameFoundation::BuffType& bt) {
	m_xHud.GetBuffPanel().AddBuff(bt);
	m_xCompletionInfo.IncrementBuffsDeployed();
}

void Level::BuffTriggerHandler(const HudBuffPanel& sender, const GameFoundation::BuffType& bt) {
	switch (bt) {
		case GameFoundation::eBuffTypeMagnet:
			m_xGame.ActivateMagnetBuff();
			break;
		case GameFoundation::eBuffTypeShield:
			m_xGame.ActivateShieldBuff();
			break;
		case GameFoundation::eBuffTypeShoot:
			m_xGame.ActivateShootBuff();
			break;
		default:
			IwAssertMsg(MYAPP, false, ("Unknown buff type: %i", bt));
			break;
	}
	m_xCompletionInfo.IncrementBuffsUsed();
}
