#ifndef _CONFIGURATION_H_ 
#define _CONFIGURATION_H_ 

#include <string>

class Configuration {
private:
	static Configuration* s_pxInstance;
	Configuration();

public:
	static void Initialize();
	static void Terminate();

	static Configuration& GetInstance();

public:
	bool ShowStats;
	bool ShowShapes;
	bool UnlockAll;

	int ResourceHeapSize;

	float MaxVisibleWorldSize;
	float WorldMargin;
	float Gravity;

	int StarBirthDelay;
	
	float BuffSpeed;
	int BuffShieldDuration;
	int BuffMagnetDuration;
	int BuffShootDuration;
	int BuffShootCount;
	
	float PathSpeed;
	float PathMaxLength;

	std::string BodiesFile;
	std::string LevelsFile;

	std::string IntroMovie;
	std::string MenuSong;
	std::string LevelSong;

	std::string AppFont;
	std::string SysFont;
	
	std::string HttpBodiesFile;
	std::string HttpLevelsFile;
	
	std::string FacebookPage;
	
	std::string SettingsFile;

	std::string FlurryKey;
	std::string LeaderboardKey;
	
	std::string AchievementFullLifeCompletionsKey;
	std::string AchievementBirdKillsKey;
	std::string AchievementBuffMagnetsKey;
	std::string AchievementBuffShieldsKey;
	std::string AchievementBuffShotsKey;

	int AchievementFullLifeCompletionsValue;
	int AchievementBirdKillsValue;
	int AchievementBuffMagnetsValue;
	int AchievementBuffShieldsValue;
	int AchievementBuffShotsValue;
	
private:
	bool Read(float& val, const std::string& key, float def = 0.0f);
	bool Read(bool& val, const std::string& key, bool def = false);
	bool Read(int& val, const std::string& key, int def = 0);
	bool Read(std::string& val, const std::string& key, std::string def = "");
};

#endif
