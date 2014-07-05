#include "LevelCompletion.h"
#include "FactoryManager.h"
#include "GameFoundation.h"
#include "Configuration.h"
#include "UserSettings.h"

#include "Debug.h"

LevelCompletion::LevelCompletion(const std::string levelid, const std::string nextlevelid, const LevelCompletionInfo& info) :
	Page("levelcompletion.group", info.IsCleared() ? Configuration::GetInstance().LevelSong : Configuration::GetInstance().LevelSong),
m_xButtonStar(info.IsCleared() ? eButtonCommandIdOpenNextLevel : eButtonCommandIdOpenLevelMenu, s3eKeyFirst),
    m_xButtonQuit(eButtonCommandIdOpenLevelMenu, s3eKeyAbsGameD),
	m_xButtonRetry(eButtonCommandIdRestartLevel, s3eKeyAbsGameB),
	m_xButtonNext(eButtonCommandIdOpenNextLevel, s3eKeyAbsRight),
    m_sLevelId(levelid),
	m_sNextLevelId(nextlevelid),
    m_xCompletionInfo(info) {

	m_xEventTimer.Elapsed.AddListener(this, &LevelCompletion::EventTimerEventHandler);
}

LevelCompletion::~LevelCompletion() {
	m_xEventTimer.Elapsed.RemoveListener(this, &LevelCompletion::EventTimerEventHandler);
}

void LevelCompletion::Initialize() {
	SaveResults();

	m_xButtonStar.SetTexture(FactoryManager::GetTextureFactory().Create("button_completion"));
    m_xButtonStar.SetTextureFrame(m_xCompletionInfo.IsCleared() ? "won" : "lost");
	m_xButtonStar.SetShadedWhenPressed(false);
	m_xButtonStar.SetRenderingLayer(Renderer::eRenderingLayerGameBackground);
    
	m_xButtonQuit.SetTexture(FactoryManager::GetTextureFactory().Create("button_pause_menu"));
	m_xButtonNext.SetTexture(FactoryManager::GetTextureFactory().Create("button_next"));
	m_xButtonRetry.SetTexture(FactoryManager::GetTextureFactory().Create("button_pause_retry"));

	m_xButtonNext.SetEnabled(m_xCompletionInfo.IsCleared());
	
	m_xBackground.Initialize();
	
	m_xTitle.SetFont(Renderer::eFontTypeLarge);
	m_xTitle.SetColour(GAME_COLOUR_FONT_MAIN);
	
	m_xScoreText.SetText("Score:");
	m_xScoreText.SetFont(Renderer::eFontTypeNormal);
	m_xScoreText.SetColour(GAME_COLOUR_FONT_MAIN);
	
	m_xScoreAmount.SetNumber(0);
	m_xScoreAmount.SetFont(Renderer::eFontTypeNormal);
	m_xScoreAmount.SetColour(GAME_COLOUR_FONT_MAIN);
	
	m_xBonusText.SetText("");
	m_xBonusText.SetFont(Renderer::eFontTypeSmall);
	m_xBonusText.SetColour(GAME_COLOUR_FONT_MAIN);

	m_xBonusAmount.SetNumber(0);
	m_xBonusAmount.SetFont(Renderer::eFontTypeSmall);
	m_xBonusAmount.SetColour(GAME_COLOUR_FONT_BONUS);
	
	ScheduleEvents();
}

void LevelCompletion::ScheduleEvents() {
	int dustamount = GetCompletionInfo().GetDustAmount();
	ScheduleTitle(GetCompletionText());
	SchedulePoints();
	ScheduleBonus("Dust collected", dustamount);
	ScheduleBonus("All nuggets collected (bonus 20%)", dustamount / 5);
	ScheduleBonus("All enemies dodged (bonus 20%)", dustamount / 5);
	ScheduleBonus("All enemies killed (bonus 50%)", dustamount / 2);
	ScheduleBonus("No buffs used (bonus 10%)", dustamount / 10);
	ScheduleStars(1);
}

void LevelCompletion::ScheduleTitle(const std::string& title) {
	EventArgs args;
	args.type = eEventTypeSetTitle;
	args.text = title;
	m_xEventTimer.Enqueue(500, args);
}

void LevelCompletion::SchedulePoints() {
	EventArgs args;
	args.type = eEventTypeSetScore;
	args.text = "Score:";
	args.amount = 0;
	m_xEventTimer.Enqueue(500, args);
}

void LevelCompletion::ScheduleBonus(const std::string& name, int amount) {
	if (amount == 0) {
		return;
	}
	
	EventArgs args;
	args.type = eEventTypeSetBonus;
	args.text = name;
	args.amount = amount;
	m_xEventTimer.Enqueue(400, args);

	args.type = eEventTypeTransferBonus;
	m_xEventTimer.Enqueue(800, args);

	args.type = eEventTypeClearBonus;
	m_xEventTimer.Enqueue(1000, args);
}

void LevelCompletion::ScheduleStars(int count) {
	EventArgs args;
	args.type = eEventTypeSetStars;
	args.amount = count;
	m_xEventTimer.Enqueue(1000, args);
}

const LevelCompletionInfo& LevelCompletion::GetCompletionInfo() {
	return m_xCompletionInfo;
}

std::string LevelCompletion::GetCompletionText() {
	return m_xCompletionInfo.IsCleared() ? "You're the best!" : "Try again";
}

void LevelCompletion::SaveResults() {
	UserSettings& settings = UserSettings::GetInstance();
	UserSettings::LevelSetting& levelsettings = settings.GetLevel(m_sLevelId);

	levelsettings.PlayCount++;

	// save scores for current level
	if (m_xCompletionInfo.IsCleared()) {
		levelsettings.Stars = 1; // just dummy evaluation
		if (levelsettings.HighScore < m_xCompletionInfo.GetDustAmount()) {
			levelsettings.HighScore = m_xCompletionInfo.GetDustAmount();
		}
	}
	settings.Save();
}

void LevelCompletion::OnDoLayout(const CIwSVec2& screensize) {
	int extents = GetScreenExtents();
	int margin = extents / 4;	// 25%
	int space = margin / 5;		// 25% / 5 = 5%
	CIwSVec2 screencenter(screensize.x / 2, screensize.y / 2);

	// star button
	CIwRect button;
	button.h = extents / 2;
	button.w = button.h;
	button.x = screencenter.x - (button.h / 2);
	button.y = screencenter.y - (button.w / 2);
	m_xButtonStar.SetPosition(button);
    
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
	m_xTitle.SetPosition(CIwRect(0, 0, screensize.x, screensize.y / 2));
	
	CIwRect textrect;
	textrect.Make(screencenter.x - (extents / 2), extents / 3, extents / 2, extents / 15);
	CIwRect numberrect(textrect);
	numberrect.x = screencenter.x;
	
	m_xScoreText.SetPosition(textrect);
	m_xScoreAmount.SetPosition(numberrect);
	
	textrect.y += textrect.h + (textrect.h / 2);
	numberrect.y = textrect.y;
	
	m_xBonusText.SetPosition(textrect);
	m_xBonusAmount.SetPosition(numberrect);
}

void LevelCompletion::OnUpdate(const FrameData& frame) {
	m_xEventTimer.Update(frame.GetSimulatedDurationMs());
	
	m_xBackground.Update(frame);
	
	m_xButtonStar.Update(frame);
	m_xButtonQuit.Update(frame);
	m_xButtonRetry.Update(frame);
	m_xButtonNext.Update(frame);

	m_xTitle.Update(frame);
	
	m_xScoreText.Update(frame);
	m_xScoreAmount.Update(frame);
	
	m_xBonusText.Update(frame);
	m_xBonusAmount.Update(frame);
}

void LevelCompletion::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xBackground.Render(renderer, frame);

	m_xButtonStar.Render(renderer, frame);
	m_xButtonQuit.Render(renderer, frame);
	m_xButtonRetry.Render(renderer, frame);
	m_xButtonNext.Render(renderer, frame);

	m_xTitle.Render(renderer, frame);
	
	m_xScoreText.Render(renderer, frame);
	m_xScoreAmount.Render(renderer, frame);
	
	m_xBonusText.Render(renderer, frame);
	m_xBonusAmount.Render(renderer, frame);
}

void LevelCompletion::EventTimerEventHandler(const MulticastEventTimer<EventArgs>& sender, const EventArgs& args) {
	switch (args.type) {
		case eEventTypeNoOp:
			break;
		case eEventTypeSetTitle:
			m_xTitle.SetText(args.text);
			break;
		case eEventTypeSetScore:
			m_xScoreText.SetText(args.text);
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
			m_xScoreAmount.SetNumber(m_xScoreAmount.GetNumber() + m_xBonusAmount.GetNumber(), 500);
			m_xBonusAmount.SetNumber(0, 500);
			break;
		case eEventTypeSetStars:
			IwAssert(MYAPP, false);
			break;
	}
}

