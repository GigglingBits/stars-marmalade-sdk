#ifndef __LEADERBOARDSNULL_H__
#define __LEADERBOARDSNULL_H__

#include "Leaderboards.h"

class LeaderboardsNull : public Leaderboards {
public:
	virtual const std::string& GetMediaName();

	virtual bool IsAvailable();
	virtual bool IsAuthenticated();
	
	virtual bool WaitForAuthentication(uint16 milliseconds);
	
	virtual void SaveScore(const std::string& leaderboardid, unsigned long score);
	virtual bool ShowLeaderboard(const std::string& leaderboardid);

	virtual void SaveAchievement(const std::string& achievementid);
	virtual bool ShowAchievements();
};

#endif
