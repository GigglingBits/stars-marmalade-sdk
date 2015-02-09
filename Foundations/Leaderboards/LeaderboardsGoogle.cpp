#include "IwDebug.h"

#include "LeaderboardsGoogle.h"

#include "Debug.h"

LeaderboardsGoogle::LeaderboardsGoogle() : m_bIsInitialized(false), m_bIsAuthenticated(false) {
	if (!IsAvailable()) {
		return;
	}
	
	s3eGooglePlayServicesRegister(S3E_GOOGLEPLAYSERVICES_SIGNIN_CALLBACK, (s3eCallback)SignedInCallback, this);
	s3eGooglePlayServicesRegister(S3E_GOOGLEPLAYSERVICES_SIGNOUT_CALLBACK, (s3eCallback)SignedOutCallback, this);
	s3eGooglePlayServicesRegister(S3E_GOOGLEPLAYSERVICES_ERROR_CALLBACK, (s3eCallback)ErrorCallback, this);
	
	Authenticate();
}

LeaderboardsGoogle::~LeaderboardsGoogle() {
	s3eGooglePlayServicesUnRegister(S3E_GOOGLEPLAYSERVICES_ERROR_CALLBACK, (s3eCallback)ErrorCallback);
	s3eGooglePlayServicesUnRegister(S3E_GOOGLEPLAYSERVICES_SIGNOUT_CALLBACK, (s3eCallback)SignedOutCallback);
	s3eGooglePlayServicesUnRegister(S3E_GOOGLEPLAYSERVICES_SIGNIN_CALLBACK, (s3eCallback)SignedInCallback);
}

const std::string& LeaderboardsGoogle::GetMediaName() {
	static const std::string s("Google Play Services");
	return s;
}

bool LeaderboardsGoogle::IsAvailable() {
	return s3eGooglePlayServicesAvailable();
}

bool LeaderboardsGoogle::IsAuthenticated() {
	return m_bIsAuthenticated;
}

bool LeaderboardsGoogle::WaitForAuthentication(uint16 milliseconds) {
	IW_CALLSTACK_SELF;
	
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
		s3eGooglePlayServicesSubmitScore(leaderboardid.c_str(), score, false);
	}
}

bool LeaderboardsGoogle::ShowLeaderboard(const std::string& leaderboardid) {
	IW_CALLSTACK_SELF;

	if (!IsAuthenticated()) {
		IwError(("Goggle Play Services not authenticated. Cannot show Leaderboards."));
		return false;
	}

	if (s3eGooglePlayServicesShowLeaderboard(leaderboardid.c_str()) == S3E_RESULT_ERROR) {
		IwAssertMsg(LEADERBOARDS, false, ("Cannot open Google Play Services Leaderboards UI"));
		return false;
	}

	return true;
}

void LeaderboardsGoogle::SaveAchievement(const std::string& achievementid, uint8 percent) {
	IW_CALLSTACK_SELF;

	if (IsAuthenticated()) {
		s3eGooglePlayServicesUnlockAchievement(achievementid.c_str(), false);
	}
}

bool LeaderboardsGoogle::ShowAchievements() {
	if (!IsAuthenticated()) {
		IwError(("Google Play Services not authenticated. Cannot show achievements."));
		return false;
	}
	
	if (s3eGooglePlayServicesShowAchievements() == S3E_RESULT_ERROR) {
		IwAssertMsg(LEADERBOARDS, false, ("Cannot open Game Center Achievements UI"));
		return false;
	}
	
	return true;
}

void LeaderboardsGoogle::Authenticate() {
	IW_CALLSTACK_SELF;
	
	if (!IsAuthenticated()) {
		s3eGooglePlayServicesSignIn();
	}
}

void LeaderboardsGoogle::SignedInCallback(void* systemData, void* userData) {
	IW_CALLSTACK_SELF;
	
	LeaderboardsGoogle* ld = (LeaderboardsGoogle*)userData;
	if (ld) {
		ld->m_bIsAuthenticated = true;
	} else {
		IwAssertMsg(LEADERBOARDS, false, ("Invalid leaderboard handle!"));
	}
}

void LeaderboardsGoogle::SignedOutCallback(void* systemData, void* userData) {
	IW_CALLSTACK_SELF;
	
	LeaderboardsGoogle* ld = (LeaderboardsGoogle*)userData;
	if (ld) {
		ld->m_bIsAuthenticated = false;
	} else {
		IwAssertMsg(LEADERBOARDS, false, ("Invalid leaderboard handle!"));
	}
}

void LeaderboardsGoogle::ErrorCallback(void* systemData, void* userData) {
	IW_CALLSTACK_SELF;
	
	LeaderboardsGoogle* ld = (LeaderboardsGoogle*)userData;
	if (ld) {
		if (s3eGooglePlayServicesErrorInfo* error = (s3eGooglePlayServicesErrorInfo*)systemData) {
			s3eDebugTracePrintf(">>>>>>>>>>> ErrorCallback, error = %d (%s)", error->error, error->errorString);
			if (error->error == S3E_GOOGLEPLAYSERVICES_ERROR_SIGNING_IN) {
				ld->m_bIsAuthenticated = false;
			}
		}
	} else {
		IwAssertMsg(LEADERBOARDS, false, ("Invalid leaderboard handle!"));
	}
}

const char* LeaderboardsGoogle::ErrorAsString(s3eGooglePlayServicesError error) {
	switch(error) {
		case S3E_GOOGLEPLAYSERVICES_ERROR_LOAD_ACHIEVEMENTS:
			return "S3E_GOOGLEPLAYSERVICES_ERROR_LOAD_ACHIEVEMENTS";
		case S3E_GOOGLEPLAYSERVICES_ERROR_LOAD_CURRENT_PLAYER_LEADERBOARD_SCORE:
			return "S3E_GOOGLEPLAYSERVICES_ERROR_LOAD_CURRENT_PLAYER_LEADERBOARD_SCORE";
		case S3E_GOOGLEPLAYSERVICES_ERROR_LOAD_PLAYER_CENTERED_SCORES:
			return "S3E_GOOGLEPLAYSERVICES_ERROR_LOAD_PLAYER_CENTERED_SCORES";
		case S3E_GOOGLEPLAYSERVICES_ERROR_LOAD_TOP_SCORES:
			return "S3E_GOOGLEPLAYSERVICES_ERROR_LOAD_TOP_SCORES";
		case S3E_GOOGLEPLAYSERVICES_ERROR_SUBMIT_SCORE:
			return "S3E_GOOGLEPLAYSERVICES_ERROR_SUBMIT_SCORE";
		case S3E_GOOGLEPLAYSERVICES_ERROR_SIGNING_IN:
			return "S3E_GOOGLEPLAYSERVICES_ERROR_SIGNING_IN";
		case S3E_GOOGLEPLAYSERVICES_ERROR_UNLOCK_ACHIEVEMENT:
			return "S3E_GOOGLEPLAYSERVICES_ERROR_UNLOCK_ACHIEVEMENT";
		case S3E_GOOGLEPLAYSERVICES_ERROR_INCREMENT_ACHIEVEMENT:
			return "S3E_GOOGLEPLAYSERVICES_ERROR_INCREMENT_ACHIEVEMENT";
		case S3E_GOOGLEPLAYSERVICES_ERROR_REVEAL_ACHIEVEMENT:
			return "S3E_GOOGLEPLAYSERVICES_ERROR_REVEAL_ACHIEVEMENT";
		default:
			return "DEVICE/UNKNOWN";
	}
}


