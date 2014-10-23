#ifndef __LEADERBOARDS_H__
#define __LEADERBOARDS_H__

#include <string>

#include "s3eIOSGameCenter.h"

class Leaderboards {
private:
	bool m_bIsAuthenticated;
	
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
	
	void SaveScore(const std::string& leaderboardid, unsigned long score);
	void ShowLeaderboard(const std::string& leaderboardid);

private:
	static void OnAuthentication(s3eIOSGameCenterError* error, void* userData);

	static const char* ErrorAsString(s3eIOSGameCenterError error);
};

#endif
