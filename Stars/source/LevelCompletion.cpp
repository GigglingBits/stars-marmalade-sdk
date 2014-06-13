#include <sstream>

#include "LevelCompletion.h"
#include "FactoryManager.h"
#include "GameFoundation.h"
#include "Configuration.h"
#include "UserSettings.h"

#include "Debug.h"

LevelCompletion::LevelCompletion(const std::string levelid, const std::string nextlevelid, const LevelCompletionInfo& info) :
	Page("levelcompletion.group", info.IsCleared() ? Configuration::GetInstance().LevelSong : Configuration::GetInstance().LevelSong),
    m_xButtonStar(eButtonCommandIdNone, s3eKeyFirst),
    m_xButtonQuit(eButtonCommandIdOpenLevelMenu, s3eKeyAbsGameD),
	m_xButtonRetry(eButtonCommandIdRestartLevel, s3eKeyAbsGameB),
	m_xButtonNext(eButtonCommandIdOpenNextLevel, s3eKeyAbsRight),
    m_sLevelId(levelid),
	m_sNextLevelId(nextlevelid),
    m_xCompletionInfo(info) {
}

void LevelCompletion::Initialize() {
	m_xButtonStar.SetTexture(FactoryManager::GetTextureFactory().Create("button_completion"));
    m_xButtonStar.SetTextureFrame(m_xCompletionInfo.IsCleared() ? "won" : "lost");
	m_xButtonStar.SetShadedWhenPressed(false);
    
	m_xButtonQuit.SetTexture(FactoryManager::GetTextureFactory().Create("button_pause_menu"));
	m_xButtonNext.SetTexture(FactoryManager::GetTextureFactory().Create("button_next"));
	m_xButtonRetry.SetTexture(FactoryManager::GetTextureFactory().Create("button_pause_retry"));

	m_xButtonNext.SetEnabled(m_xCompletionInfo.IsCleared());
	
	m_xBackground.Initialize();

	SaveResults();
	
	m_xTitle.SetText(GetCompletionText());
	m_xTitle.SetFont(Renderer::eFontTypeLarge);
	m_xTitle.SetColour(GAME_COLOUR_FONT_MAIN);
	
	m_xDustAmountText.SetText("Dust collected:");
	m_xDustAmountText.SetFont(Renderer::eFontTypeNormal);
	m_xDustAmountText.SetColour(GAME_COLOUR_FONT_MAIN);
	
	m_xDustAmount.SetNumber(m_xCompletionInfo.GetDustAmount(), 5000);
	m_xDustAmount.SetFont(Renderer::eFontTypeNormal);
	m_xDustAmount.SetColour(GAME_COLOUR_FONT_MAIN);
	
	m_xNuggetsCollectedText.SetText("Nuggets collected:");
	m_xNuggetsCollectedText.SetFont(Renderer::eFontTypeNormal);
	m_xNuggetsCollectedText.SetColour(GAME_COLOUR_FONT_MAIN);

	m_xNuggetsCollected.SetNumber(m_xCompletionInfo.GetNuggetsCollected(), 5000);
	m_xNuggetsCollected.SetFont(Renderer::eFontTypeNormal);
	m_xNuggetsCollected.SetColour(GAME_COLOUR_FONT_MAIN);

	m_xNumberOfPathsText.SetText("Paths drawn:");
	m_xNumberOfPathsText.SetFont(Renderer::eFontTypeNormal);
	m_xNumberOfPathsText.SetColour(GAME_COLOUR_FONT_MAIN);

	m_xNumberOfPaths.SetNumber(m_xCompletionInfo.GetPathsStarted(), 5000);
	m_xNumberOfPaths.SetFont(Renderer::eFontTypeNormal);
	m_xNumberOfPaths.SetColour(GAME_COLOUR_FONT_MAIN);
}

const LevelCompletionInfo& LevelCompletion::GetCompletionInfo() {
	return m_xCompletionInfo;
}

std::string LevelCompletion::GetCompletionText() {
	std::ostringstream oss;
	if (m_xCompletionInfo.IsCleared()) {
		oss << "Congratulations!" << std::endl;
	} else {
		oss << "Try again" << std::endl;
	}
	return oss.str();
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
	
	m_xDustAmountText.SetPosition(textrect);
	m_xDustAmount.SetPosition(numberrect);
	
	textrect.y += textrect.h + (textrect.h / 2);
	numberrect.y = textrect.y;
	
	m_xNuggetsCollectedText.SetPosition(textrect);
	m_xNuggetsCollected.SetPosition(numberrect);

	textrect.y += textrect.h + (textrect.h / 2);
	numberrect.y = textrect.y;

	m_xNumberOfPathsText.SetPosition(textrect);
	m_xNumberOfPaths.SetPosition(numberrect);
}

void LevelCompletion::OnUpdate(const FrameData& frame) {
	m_xBackground.Update(frame);
	
	m_xButtonStar.Update(frame);
	m_xButtonQuit.Update(frame);
	m_xButtonRetry.Update(frame);
	m_xButtonNext.Update(frame);

	m_xTitle.Update(frame);
	
	m_xDustAmountText.Update(frame);
	m_xDustAmount.Update(frame);
	
	m_xNuggetsCollectedText.Update(frame);
	m_xNuggetsCollected.Update(frame);
	
	m_xNumberOfPathsText.Update(frame);
	m_xNumberOfPaths.Update(frame);
}

void LevelCompletion::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xBackground.Render(renderer, frame);

	m_xTitle.Render(renderer, frame);
	
	m_xDustAmountText.Render(renderer, frame);
	m_xDustAmount.Render(renderer, frame);
	
	m_xNuggetsCollectedText.Render(renderer, frame);
	m_xNuggetsCollected.Render(renderer, frame);
	
	m_xNumberOfPathsText.Render(renderer, frame);
	m_xNumberOfPaths.Render(renderer, frame);
	
	// buttons
	m_xButtonStar.Render(renderer, frame);
	m_xButtonQuit.Render(renderer, frame);
	m_xButtonRetry.Render(renderer, frame);
	m_xButtonNext.Render(renderer, frame);
}
