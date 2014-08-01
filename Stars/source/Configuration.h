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

	double MaxVisibleWorldSize;
	double WorldMargin;
	double Gravity;

	int StarBirthDelay;
	
	double BuffSpeed;
	int BuffShieldDuration;
	
	double PathSpeed;
	double PathMaxLength;

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
};

#endif
