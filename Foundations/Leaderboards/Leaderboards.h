#ifndef __LEADERBOARDS_H__
#define __LEADERBOARDS_H__

#include <string>

#include "s3eIOSGameCenter.h"

class Leaderboards {
private:
	static Leaderboards* s_pxInstance;
	Leaderboards();
	~Leaderboards();
	
public:
	static void Initialize();
	static void Terminate();
	static Leaderboards& GetInstance();
	
public:
	bool IsAvailable();
	bool IsAuthenticated();
	
	bool WaitForAuthentication(uint16 milliseconds);
	
	void SaveScore(const std::string& leaderboardid, unsigned long score);
	bool ShowLeaderboard(const std::string& leaderboardid);

private:
	void Authenticate();

	static void AuthenticationCallback(s3eIOSGameCenterError* error, void* userData);
	static void SaveScoreCallback(s3eIOSGameCenterError* error);

	static const char* ErrorAsString(s3eIOSGameCenterError error);
};

#endif
