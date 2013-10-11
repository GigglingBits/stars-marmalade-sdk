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

	double MaxVisibleWorldSize;
	double WorldMargin;

	double ObjectSpeed;

	double PathSpeed;
	double PathMaxLength;

	std::string BodiesFile;
	std::string LevelsFile;

	std::string IntroSong;
	std::string LevelSong;
	std::string WonSong;
	std::string LostSong;

	std::string HttpBodiesFile;
	std::string HttpLevelsFile;
};

#endif
