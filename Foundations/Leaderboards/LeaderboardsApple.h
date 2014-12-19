#ifndef __LEADERBOARDSAPPLE_H__
#define __LEADERBOARDSAPPLE_H__

#include <string>

#include "Leaderboards.h"

#include "s3eIOSGameCenter.h"

class LeaderboardsApple : public Leaderboards {
	friend class Leaderboards;

private:
	bool m_bIsAuthenticating;
	
protected:
	LeaderboardsApple();
	virtual ~LeaderboardsApple();
	
public:
	virtual const std::string& GetMediaName();
	
	virtual bool IsAvailable();
	virtual bool IsAuthenticated();
	
	virtual bool WaitForAuthentication(uint16 milliseconds);
	
	virtual void SaveScore(const std::string& leaderboardid, unsigned long score);
	virtual bool ShowLeaderboard(const std::string& leaderboardid);

	virtual void SaveAchievement(const std::string& achievementid, uint8 percent = 100);
	virtual bool ShowAchievements();

private:
	void Authenticate();

	static void AuthenticationCallback(s3eIOSGameCenterError* error, void* userData);
	static void SaveScoreCallback(s3eIOSGameCenterError* error);
	static void SaveAchievementCallback(s3eIOSGameCenterError* error);

	static const char* ErrorAsString(s3eIOSGameCenterError error);
};

#endif
