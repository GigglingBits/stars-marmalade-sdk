#include "LeaderboardsGoogle.h"

#include "s3eGooglePlayGames.h"

#include "IwDebug.h"
#include "Debug.h"

LeaderboardsGoogle::LeaderboardsGoogle() : m_bIsInitialized(false) {
	m_bIsInitialized = S3E_RESULT_SUCCESS == s3eGPGInitialize();
}

LeaderboardsGoogle::~LeaderboardsGoogle() {
	if (m_bIsInitialized) {
		s3eGPGDeInitialize();
		m_bIsInitialized = false;
	}
}

const std::string& LeaderboardsGoogle::GetMediaName() {
	static const std::string s("Google Play Services");
	return s;
}

bool LeaderboardsGoogle::IsAvailable() {
	return m_bIsInitialized;
}

bool LeaderboardsGoogle::IsAuthenticated() {
	return m_bIsInitialized && s3eGPGIsSignedIn();
}

bool LeaderboardsGoogle::WaitForAuthentication(uint16 milliseconds) {
	// No need to authenticate; Authentication is done in constructor
	// Authenticate();
	
	const uint16 interval = 200;
	uint16 counter = 0;
	while (!IsAuthenticated()) {
		s3eDeviceYield(interval);
		counter += interval;
		if (counter >= milliseconds) {
			return false;
		}
	}
	return true;
}

void LeaderboardsGoogle::SaveScore(const std::string& leaderboardid, unsigned long score) {
	IW_CALLSTACK_SELF;
	
	if (IsAuthenticated()) {
		s3eGPGSubmitLeaderboardScore(leaderboardid.c_str(), score);
	}
}

bool LeaderboardsGoogle::ShowLeaderboard(const std::string& leaderboardid) {
	IW_CALLSTACK_SELF;

	if (!IsAuthenticated()) {
		IwError(("Goggle Play Services not authenticated. Cannot show Leaderboards."));
		return false;
	}

	s3eGPGShowLeaderboardUI(leaderboardid.c_str());
	return true;
}

void LeaderboardsGoogle::SaveAchievement(const std::string& achievementid, uint8 percent) {
	
}

bool LeaderboardsGoogle::ShowAchievements() {
	return false;
}
