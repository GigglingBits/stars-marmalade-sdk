#include "LevelCompletion.h"
#include "FactoryManager.h"
#include "GameFoundation.h"
#include "Configuration.h"
#include "UserSettings.h"
#include "Leaderboards.h"
#include "SoundEngine.h"

#include "Debug.h"

LevelCompletion::LevelCompletion(const std::string levelid, const std::string nextlevelid, const LevelCompletionInfo& info) :
	Page("levelcompletion.group", info.IsLevelAchieved() ? Configuration::GetInstance().LevelSong : Configuration::GetInstance().LevelSong),
	m_pxStar(NULL),
	m_pxAward(NULL),
    m_xButtonQuit(eButtonCommandIdOpenLevelMenu, s3eKeyAbsGameD),
	m_xButtonRetry(eButtonCommandIdRestartLevel, s3eKeyAbsGameB),
	m_xButtonNext(eButtonCommandIdOpenNextLevel, s3eKeyAbsRight),
    m_sLevelId(levelid),
	m_sNextLevelId(nextlevelid),
    m_xCompletionInfo(info),
	m_bEnableStar(false) {
	m_xEventTimer.Elapsed.AddListener(this, &LevelCompletion::EventTimerEventHandler);
}

LevelCompletion::~LevelCompletion() {
	if (m_pxStar) {
		delete m_pxStar;
	}
	if (m_pxAward) {
		delete m_pxAward;
	}
	m_xEventTimer.Elapsed.RemoveListener(this, &LevelCompletion::EventTimerEventHandler);
}

void LevelCompletion::Initialize() {
	SaveResults();

	std::string startexture = m_xCompletionInfo.IsLevelAchieved() ? "completion_won" : "completion_lost";
	if ((m_pxStar = FactoryManager::GetTextureFactory().Create(startexture))) {
		m_pxStar->SelectFrame("main");
	}
	
	if ((m_pxAward = FactoryManager::GetTextureFactory().Create("completion_award"))) {
		m_pxAward->SelectFrame("hidden");
	}
	
	m_xButtonQuit.SetTexture(FactoryManager::GetTextureFactory().Create("button_pause_menu"));
	m_xButtonNext.SetTexture(FactoryManager::GetTextureFactory().Create("button_next"));
	m_xButtonRetry.SetTexture(FactoryManager::GetTextureFactory().Create("button_pause_retry"));

	m_xButtonNext.SetEnabled(m_xCompletionInfo.IsLevelAchieved());
	
	m_xBackground.Initialize();
	
	//m_xTitleText.SetBackground("white");
	m_xTitleText.SetText("");
	m_xTitleText.SetFont(Renderer::eFontTypeLarge);
	m_xTitleText.SetColour(GAME_COLOUR_FONT_MAIN);
	m_xTitleText.SetAlignH(Renderer::eTextAlignHCenter);
	m_xTitleText.SetAlignV(Renderer::eTextAlignVMiddle);
	
	//m_xScoreText.SetBackground("white");
	m_xScoreText.SetText("");
	m_xScoreText.SetFont(Renderer::eFontTypeLarge);
	m_xScoreText.SetColour(GAME_COLOUR_FONT_MAIN);
	m_xScoreText.SetAlignH(Renderer::eTextAlignHLeft);
	m_xScoreText.SetAlignV(Renderer::eTextAlignVMiddle);
	
	//m_xScoreAmount.SetBackground("white");
	//m_xScoreAmount.SetNumber(-1);
	m_xScoreAmount.SetText("");
	m_xScoreAmount.SetFont(Renderer::eFontTypeLarge);
	m_xScoreAmount.SetColour(GAME_COLOUR_FONT_MAIN);
	m_xScoreAmount.SetAlignH(Renderer::eTextAlignHRight);
	m_xScoreAmount.SetAlignV(Renderer::eTextAlignVMiddle);
	
	//m_xBonusText.SetBackground("white");
	m_xBonusText.SetText("");
	m_xBonusText.SetFont(Renderer::eFontTypeNormal);
	m_xBonusText.SetColour(GAME_COLOUR_FONT_MAIN);
	m_xBonusText.SetAlignH(Renderer::eTextAlignHLeft);
	m_xBonusText.SetAlignV(Renderer::eTextAlignVMiddle);

	//m_xBonusAmount.SetBackground("white");
	//m_xBonusAmount.SetNumber(-1);
	m_xBonusAmount.SetText("");
	m_xBonusAmount.SetFont(Renderer::eFontTypeNormal);
	m_xBonusAmount.SetColour(GAME_COLOUR_FONT_BONUS);
	m_xBonusAmount.SetAlignH(Renderer::eTextAlignHRight);
	m_xBonusAmount.SetAlignV(Renderer::eTextAlignVMiddle);
	
	ScheduleEvents();
	
	if (m_xCompletionInfo.IsLevelAchieved()) {
		SoundEngine::GetInstance().PlaySoundEffect("level_win");
	}
}

void LevelCompletion::ScheduleEvents() {
	
	ScheduleTitle();

	LevelCompletionInfo& ci = m_xCompletionInfo;
	if (ci.GetLivesLeft() > 0) {
		SchedulePause(2200);
		ScheduleTitleClear();
		SchedulePause(700);

		SchedulePoints();
		
		std::vector<LevelCompletionInfo::Points> points;
		ci.GetIndividualPoints(points);
		std::vector<LevelCompletionInfo::Points>::const_iterator it;
		for (it = points.begin(); it != points.end(); ++it) {
			ScheduleBonus(it->Text, it->Amount);
			SchedulePause(400);
		}

		ScheduleAwards(ci.GetAchievedStars());
	}
	
	SchedulePause(1000);
	ScheduleEnableStar();
}

void LevelCompletion::ScheduleTitle() {
	std::string msg;
	if (m_xCompletionInfo.IsLevelAchieved()) {
		msg = "Well done!";
	} else {
		msg = "Try again...";
	}
	
	EventArgs args;
	args.type = eEventTypeSetTitle;
	args.text = msg;
	m_xEventTimer.Enqueue(0, args);
}

void LevelCompletion::ScheduleTitleClear() {
	EventArgs args;
	args.type = eEventTypeSetTitle;
	args.text = "";
	m_xEventTimer.Enqueue(0, args);
}

void LevelCompletion::ScheduleEnableStar() {
	EventArgs args;
	args.type = eEventTypeEnableStar;
	m_xEventTimer.Enqueue(0, args);
}

void LevelCompletion::SchedulePause(int milliseconds) {
	EventArgs args;
	args.type = eEventTypeNoOp;
	m_xEventTimer.Enqueue(milliseconds, args);
}

void LevelCompletion::SchedulePoints() {
	EventArgs args;
	args.type = eEventTypeSetScore;
	args.text = "Score:";
	args.amount = 0;
	m_xEventTimer.Enqueue(0, args);
}

void LevelCompletion::ScheduleBonus(const std::string& name, int amount) {
	if (amount == 0) {
		return;
	}
	
	EventArgs args;
	args.type = eEventTypeSetBonus;
	args.text = name;
	args.amount = amount;
	m_xEventTimer.Enqueue(0, args);

	args.type = eEventTypeTransferBonus;
	m_xEventTimer.Enqueue(1200, args);

	args.type = eEventTypeClearBonus;
	m_xEventTimer.Enqueue(1300, args);
}

void LevelCompletion::ScheduleAwards(int starcount) {
	EventArgs args;
	args.type = eEventTypeClearBonus;
	m_xEventTimer.Enqueue(0, args);
	
	args.type = eEventTypeGiveAwards;
	args.amount = starcount;
	m_xEventTimer.Enqueue(1000, args);

	args.type = eEventTypeNoOp;
	args.amount = starcount;
	m_xEventTimer.Enqueue(2500, args);
}

const LevelCompletionInfo& LevelCompletion::GetCompletionInfo() {
	return m_xCompletionInfo;
}

void LevelCompletion::SaveResults() {
	UserSettings& settings = UserSettings::GetInstance();
	UserSettings::LevelSetting& levelsettings = settings.GetLevel(m_sLevelId);

	// update specific level
	levelsettings.PlayCount++;
	int stars = m_xCompletionInfo.GetAchievedStars();
	if (levelsettings.Stars < stars) {
		levelsettings.Stars = stars;
	}
	int score = m_xCompletionInfo.GetTotalPoints();
	if (levelsettings.HighScore < score) {
		levelsettings.HighScore = score;
	}
	int livesused = m_xCompletionInfo.GetLivesUsed();
	if (livesused == 0 && levelsettings.FullLifeCompletions < std::numeric_limits<int>::max()) {
			levelsettings.FullLifeCompletions ++;
	}
	int birdskilled = m_xCompletionInfo.GetBirdsKilled();
	if (birdskilled > 0 && levelsettings.BirdKills < std::numeric_limits<int>::max() - birdskilled) {
		levelsettings.BirdKills += birdskilled;
	}
	int buffmagnetsused = m_xCompletionInfo.GetBuffMagnetsUsed();
	if (buffmagnetsused > 0 && levelsettings.BuffMagnetsUsed < std::numeric_limits<int>::max() - buffmagnetsused) {
		levelsettings.BuffMagnetsUsed += buffmagnetsused;
	}
	int buffshieldsused = m_xCompletionInfo.GetBuffShieldsUsed();
	if (buffshieldsused > 0 && levelsettings.BuffShieldsUsed < std::numeric_limits<int>::max() - buffshieldsused) {
		levelsettings.BuffShieldsUsed += buffshieldsused;
	}
	int buffshotsused = m_xCompletionInfo.GetBuffShotsUsed();
	if (buffshotsused > 0 && levelsettings.BuffShotsUsed < std::numeric_limits<int>::max() - buffshotsused) {
		levelsettings.BuffShotsUsed += buffshotsused;
	}
	settings.Save();
	
	// update total score
	Leaderboards& ld = Leaderboards::GetInstance();
	ld.SaveScore(Configuration::GetInstance().LeaderboardKey, settings.GetTotalScore());
}

void LevelCompletion::OnDoLayout(const CIwSVec2& screensize) {
	int extents = GetScreenExtents();

	int margin = extents / 4;	// 25%
	int space = margin / 5;		// 25% / 5 = 5%

	int backdropwidth = extents * 0.7;

	CIwSVec2 screencenter(screensize.x / 2, screensize.y / 2);

	// star button
	CIwRect button;
	button.h = extents / 4;
	button.w = button.h;
	button.x = screencenter.x - (button.h / 2);
	button.y = screencenter.y - (button.w / 2);
	m_xStarShape.SetRect(button);

	// awards
	m_xAwardShape.SetRect(button);

    // the button size is determined by the screen height
	button.h = extents / 7;
	button.w = button.h;
    
	// positions
	button.x = (screensize.x - (3 * button.w) - (2 * space)) / 2;
	button.y = screensize.y * 10 / 13;
	m_xButtonQuit.SetPosition(button);
	
	button.x += button.w + space;
	m_xButtonRetry.SetPosition(button);
	
	button.x += button.w + space;
	m_xButtonNext.SetPosition(button);
	
	// text boxes
	CIwRect rect;
	rect.Make(screencenter.x - (backdropwidth / 2), extents / 10, backdropwidth, extents / 15);

	m_xTitleText.SetPosition(rect);
	m_xScoreText.SetPosition(rect);
	m_xScoreAmount.SetPosition(rect);
	
	rect.y += rect.h + rect.h;
	
	m_xBonusText.SetPosition(rect);
	m_xBonusAmount.SetPosition(rect);
	
	// readability
	m_xBackdropShape.SetRect(0, 0, screensize.x, screensize.y);
}

void LevelCompletion::OnUpdate(const FrameData& frame) {
	m_xEventTimer.Update(frame.GetSimulatedDurationMs());
	
	m_xBackground.Update(frame);

	if (m_bEnableStar && m_pxStar) {
		m_pxStar->Update(frame.GetRealDurationMs());
	}
	if (m_pxAward) {
		m_pxAward->Update(frame.GetRealDurationMs());
	}
	
	m_xButtonQuit.Update(frame);
	m_xButtonRetry.Update(frame);
	m_xButtonNext.Update(frame);
	
	m_xTitleText.Update(frame);

	m_xScoreText.Update(frame);
	m_xScoreAmount.Update(frame);
	
	m_xBonusText.Update(frame);
	m_xBonusAmount.Update(frame);
}

void LevelCompletion::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xBackground.Render(renderer, frame);
	renderer.DrawPolygon(m_xBackdropShape.GetVerts(), m_xBackdropShape.GetVertCount(), 0x00000000, 0x33000000);
	
	if (m_bEnableStar && m_pxStar) {
		renderer.Draw(m_xStarShape, *m_pxStar);
	}
	if (m_pxAward) {
		renderer.Draw(m_xAwardShape, *m_pxAward);
	}
	
	m_xButtonQuit.Render(renderer, frame);
	m_xButtonRetry.Render(renderer, frame);
	m_xButtonNext.Render(renderer, frame);
	
	m_xTitleText.Render(renderer, frame);
	
	m_xScoreText.Render(renderer, frame);
	m_xScoreAmount.Render(renderer, frame);
	
	m_xBonusText.Render(renderer, frame);
	m_xBonusAmount.Render(renderer, frame);
}

void LevelCompletion::EventTimerEventHandler(const MulticastEventTimer<EventArgs>& sender, const EventArgs& args) {
	IW_CALLSTACK_SELF;
	
	switch (args.type) {
		case eEventTypeNoOp:
			break;
		case eEventTypeSetTitle:
			m_xTitleText.SetText(args.text);
			m_xScoreText.SetText("");
			m_xScoreAmount.SetText("");
			break;
		case eEventTypeSetScore:
			m_xTitleText.SetText("");
			m_xScoreText.SetText(args.text);
			m_xScoreAmount.SetNumber(-1); // number is not visible initially if it is set to 0; force it != zero to make it visible
			m_xScoreAmount.SetNumber(args.amount);
			break;
		case eEventTypeClearBonus:
			m_xBonusText.SetText("");
			m_xBonusAmount.SetText("");
			break;
		case eEventTypeSetBonus:
			m_xBonusText.SetText(args.text);
			m_xBonusAmount.SetNumber(args.amount);
			break;
		case eEventTypeTransferBonus:
			m_xScoreAmount.SetNumber(m_xScoreAmount.GetNumber() + m_xBonusAmount.GetNumber(), 800);
			m_xBonusAmount.SetNumber(0, 800);
			break;
		case eEventTypeGiveAwards:
			if (m_pxAward) {
				std::ostringstream oss;
				oss << args.amount;
				m_pxAward->SelectFrame(oss.str());
			}
			break;
		case eEventTypeEnableStar:
			m_bEnableStar = true;
			break;
	}
}

