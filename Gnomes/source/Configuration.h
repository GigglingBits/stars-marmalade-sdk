#ifndef _CONFIGURATION_H_ 
#define _CONFIGURATION_H_ 

#include <string>

class Configuration {
private:
	static Configuration* m_pxInstance;
	Configuration();

public:
	static void Initialize();
	static void Terminate();

	static Configuration& GetInstance();

public:
	bool ShowStats;

	double MaxVisibleWorldSize;
	double WorldMargin;

	std::string BodiesFile;
	std::string LevelsFile;

	std::string HttpBodiesFile;
	std::string HttpLevelsFile;
};

#endif
