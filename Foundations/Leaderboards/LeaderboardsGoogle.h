#ifndef __LEADERBOARDSGOOGLE_H__
#define __LEADERBOARDSGOOGLE_H__


#include <string>

#include "Leaderboards.h"

class LeaderboardsGoogle : public Leaderboards {
	friend class Leaderboards;

private:
	bool m_bIsInitialized;
	
protected:
	LeaderboardsGoogle();
	virtual ~LeaderboardsGoogle();
	
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
