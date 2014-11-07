#include "LeaderboardsApple.h"

#include "IwDebug.h"
#include "Debug.h"


LeaderboardsApple::LeaderboardsApple() {
	if (!IsAvailable()) {
		return;
	}

	Authenticate();
}

LeaderboardsApple::~LeaderboardsApple() {
}

bool LeaderboardsApple::IsAvailable() {
	return s3eIOSGameCenterAvailable();
}

bool LeaderboardsApple::IsAuthenticated() {
	IW_CALLSTACK_SELF;

	return s3eIOSGameCenterGetInt(S3E_IOSGAMECENTER_LOCAL_PLAYER_IS_AUTHENTICATED) ? true : false;
}

bool LeaderboardsApple::WaitForAuthentication(uint16 milliseconds) {
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

void LeaderboardsApple::SaveScore(const std::string& leaderboardid, unsigned long score) {
	IW_CALLSTACK_SELF;
	
	if (IsAuthenticated()) {
		s3eIOSGameCenterReportScore(score, leaderboardid.c_str(), SaveScoreCallback);
	}
}

bool LeaderboardsApple::ShowLeaderboard(const std::string& leaderboardid) {
	if (!IsAuthenticated()) {
		IwError(("Game Center not authenticated. Cannot show LeaderboardsApple."));
		return false;
	}
	
	if (s3eIOSGameCenterLeaderboardShowGUI(leaderboardid.c_str(), S3E_IOSGAMECENTER_PLAYER_SCOPE_ALL_TIME) == S3E_RESULT_ERROR) {
		IwAssertMsg(LEADERBOARDS, false, ("Cannot open Game Center UI"));
		return false;
	}
	
	return true;
}

void LeaderboardsApple::Authenticate() {
	IW_CALLSTACK_SELF;

	if (!IsAuthenticated()) {
		s3eIOSGameCenterAuthenticate(LeaderboardsApple::AuthenticationCallback, this);
	}
}

void LeaderboardsApple::AuthenticationCallback(s3eIOSGameCenterError* error, void* userData) {
	IW_CALLSTACK_SELF;
	
	LeaderboardsApple* ld = (LeaderboardsApple*)userData;
	if (!ld) {
		IwAssertMsg(LEADERBOARDS, false, ("Ivalid leaderboard handle!"));
		return;
	}
	
	IwAssertMsg(LEADERBOARDS, S3E_IOSGAMECENTER_ERR_NONE == *error, ("Game Center authentication failed: %s", ErrorAsString(*error)));
}

void LeaderboardsApple::SaveScoreCallback(s3eIOSGameCenterError* error) {
	IW_CALLSTACK_SELF;
	
	IwAssertMsg(LEADERBOARDS, S3E_IOSGAMECENTER_ERR_NONE == *error, ("Game Center score submission failed: %s", ErrorAsString(*error)));
}

const char* LeaderboardsApple::ErrorAsString(s3eIOSGameCenterError error) {
	switch(error) {
		case S3E_IOSGAMECENTER_ERR_NONE:
			return "NONE";
		case S3E_IOSGAMECENTER_ERR_PARAM:
			return "PARAM";
		case S3E_IOSGAMECENTER_ERR_UNAUTHENTICATED:
			return "UNAUTHENTICATED";
		case S3E_IOSGAMECENTER_ERR_FRIENDS_NOT_LOADED:
			return "FRIENDS_NOT_LOADED";
		case S3E_IOSGAMECENTER_ERR_ALREADY_REG:
			return "ALREADY_REG";
		case S3E_IOSGAMECENTER_ERR_GAME_UNRECOGNISED:
			return "GAME_UNRECOGNISED";
		case S3E_IOSGAMECENTER_ERR_AUTHENTICATION_IN_PROGRESS:
			return "AUTHENTICATION_IN_PROGRESS";
		case S3E_IOSGAMECENTER_ERR_INVALID_CREDENTIALS:
			return "INVALID_CREDENTIALS";
		case S3E_IOSGAMECENTER_ERR_UNDERAGE:
			return "UNDERAGE";
		case S3E_IOSGAMECENTER_ERR_COMMUNICATIONS_FAILURE:
			return "COMMUNICATIONS_FAILURE";
		case S3E_IOSGAMECENTER_ERR_CANCELLED:
			return "CANCELLED";
		case S3E_IOSGAMECENTER_ERR_USER_DENIED:
			return "DENIED";
		case S3E_IOSGAMECENTER_ERR_INVALID_PLAYER:
			return "INVALID_PLAYER";
		case S3E_IOSGAMECENTER_ERR_SCORE_NOT_SET:
			return "SCORE_NOT_SET";
		case S3E_IOSGAMECENTER_ERR_PARENTAL_CONTROLS_BLOCKED:
			return "PARENTAL_CONTROLS_BLOCKED";
		case S3E_IOSGAMECENTER_ERR_INVALID_MATCH_REQUEST:
			return "INVALID_MATCH_REQUEST";
		case S3E_IOSGAMECENTER_ERR_DEVICE:
		default:
			return "DEVICE/UNKNOWN";
	}
}

