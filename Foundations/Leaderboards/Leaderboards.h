#ifndef __LEADERBOARDS_H__
#define __LEADERBOARDS_H__

#include <string>

class Leaderboards {	
private:
	static Leaderboards* s_pxInstance;
	
protected:
	Leaderboards();
	virtual ~Leaderboards();
	
public:
	static void Initialize();
	static void Terminate();
	static Leaderboards& GetInstance();
	
public:
	virtual const std::string& GetMediaName() = 0;
	
	virtual bool IsAvailable() = 0;
	virtual bool IsAuthenticated() = 0;
	
	virtual bool WaitForAuthentication(uint16 milliseconds) = 0;
	
	virtual void SaveScore(const std::string& leaderboardid, unsigned long score) = 0;
	virtual bool ShowLeaderboard(const std::string& leaderboardid) = 0;

	virtual void SaveAchievement(const std::string& achievementid) = 0;
	virtual bool ShowAchievements() = 0;
};

#endif
