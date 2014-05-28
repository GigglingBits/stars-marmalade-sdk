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
	bool UnlockAll;

	double MaxVisibleWorldSize;
	double WorldMargin;

	double BuffSpeed;
	
	double PathSpeed;
	double PathMaxLength;

	std::string BodiesFile;
	std::string LevelsFile;

	std::string IntroMovie;
	std::string MenuSong;
	std::string LevelSong;

	std::string HttpBodiesFile;
	std::string HttpLevelsFile;
	
	std::string FacebookPage;
	
	std::string SettingsFile;
};

#endif
