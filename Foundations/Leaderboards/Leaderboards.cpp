#include "Leaderboards.h"

#include "IwDebug.h"
#include "Debug.h"

Leaderboards* Leaderboards::s_pxInstance = NULL;

void Leaderboards::Initialize() {
	IwError(("Initializing GC"));
	if (!s_pxInstance) {
		s_pxInstance = new Leaderboards();
	}
}

void Leaderboards::Terminate() {
	if (s_pxInstance) {
		delete s_pxInstance;
		s_pxInstance = NULL;
	}
	
	if (s3eIOSGameCenterAvailable()) {
		// uninit
	}
}

Leaderboards::Leaderboards() : m_bIsAuthenticated(false) {
	if (!s3eIOSGameCenterAvailable()) {
		IwError(("GC not available"));
		return;
	}
	
	m_bIsAuthenticated = s3eIOSGameCenterGetInt(S3E_IOSGAMECENTER_LOCAL_PLAYER_IS_AUTHENTICATED) ? true : false;
	if (!m_bIsAuthenticated) {
		IwError(("Authenticating GC"));
		s3eIOSGameCenterAuthenticate(Leaderboards::OnAuthentication, NULL);
	} else {
		IwError(("Already authenticated GC"));
	}
}

Leaderboards::~Leaderboards() {
}

Leaderboards& Leaderboards::GetInstance() {
	IwAssertMsg(MYAPP, s_pxInstance, ("The leaderboards are not initialized."));
	return *s_pxInstance;
}

bool Leaderboards::IsAvailable() {
	return s3eIOSGameCenterAvailable();
}

void Leaderboards::SaveScore(const std::string& leaderboardid, unsigned long score) {
	IW_CALLSTACK_SELF;
	
	if (m_bIsAuthenticated) {
		s3eIOSGameCenterReportScore(score, "", NULL);
	}
}

void Leaderboards::ShowLeaderboard(const std::string& leaderboardid) {
	IwError(("Showing GC leaderboard"));
	if (s3eIOSGameCenterLeaderboardShowGUI(leaderboardid.c_str(), S3E_IOSGAMECENTER_PLAYER_SCOPE_ALL_TIME) == S3E_RESULT_ERROR) {
		IwAssertMsg(LEADERBOARDS, false, ("Cannot open Game Center UI"));
	}
}

void Leaderboards::OnAuthentication(s3eIOSGameCenterError* error, void* userData) {
	IW_CALLSTACK_SELF;
	
	Leaderboards* ld = (Leaderboards*)userData;
	if (!ld) {
		return;
	}
	
	ld->m_bIsAuthenticated = S3E_IOSGAMECENTER_ERR_NONE == *error;
	IwAssertMsg(LEADERBOARDS, ld->m_bIsAuthenticated, ("Game Center authentication failed: %s", ErrorAsString(*error)));
}

const char* Leaderboards::ErrorAsString(s3eIOSGameCenterError error) {
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

