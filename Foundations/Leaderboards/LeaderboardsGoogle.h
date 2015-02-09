#ifndef __LEADERBOARDSGOOGLE_H__
#define __LEADERBOARDSGOOGLE_H__

#include <string>
#include "Leaderboards.h"
#include "s3eGooglePlayServices.h"

class LeaderboardsGoogle : public Leaderboards {
	friend class Leaderboards;

private:
	bool m_bIsInitialized;
	bool m_bIsAuthenticated;
	
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
	
	virtual void SaveAchievement(const std::string& achievementid, uint8 percent = 100);
	virtual bool ShowAchievements();

private:
	void Authenticate();
	
	static const char* ErrorAsString(s3eGooglePlayServicesError error);

	static void SignedInCallback(void* systemData, void* userData);
	static void SignedOutCallback(void* systemData, void* userData);
	static void ErrorCallback(void* systemData, void* userData);
};

#endif
