#ifndef _CONFIGURATION_H_ 
#define _CONFIGURATION_H_ 

#include <string>

class Configuration {
private:
	static Configuration* m_pxInstance;
	Configuration();

public:
	static void Initialize();
	static void UnInitialize();

	static Configuration& GetInstance();

public:
	bool AccelerometerStarted;

	bool ShowStats;
	bool AutoRotate;

	bool ExitButton;
	bool HelpButton;

	bool EnableSpeech;
	bool EnableText;

	float AnimationConstant;

	std::string Language;
	std::string Locale;

	std::string PageMapFile;
	std::string HelpFile;

private:
	const char* GetDeviceLanguage();
	bool SpeechFolderExists();
};

#endif
