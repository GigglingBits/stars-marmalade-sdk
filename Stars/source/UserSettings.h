#ifndef __USERSETTINGS_H__ 
#define __USERSETTINGS_H__ 

#include <string>
#include <map>

#include "tinyxml.h"

#define USER_SETTINGS_FILEFORMATVERSION_NULL 0

#define USER_SETTINGS_FILEFORMATVERSION_ATTR "fileformatversion"
#define USER_SETTINGS_FILEFORMATVERSION 1

#define USER_SETTINGS_SETTINGS_TAG "settings"
#define USER_SETTINGS_LEVELS_TAG "levels"
#define USER_SETTINGS_LEVEL_TAG "level"

#define USER_SETTINGS_ID_ATTR "id"
#define USER_SETTINGS_PLAYCOUNT_ATTR "playcount"
#define USER_SETTINGS_HIGHSCORE_ATTR "highscore"
#define USER_SETTINGS_STARS_ATTR "stars"


class UserSettings {
public:
	struct LevelSetting {
		int PlayCount;
		int HighScore;
		int Stars;
		LevelSetting() {
			PlayCount = 0;
			HighScore = 0;
			Stars = 0;
		}
	};
	
private:
	std::string m_sSettingsFileName;
	
	typedef std::map<std::string, LevelSetting> LevelSettings;
	LevelSettings m_xLevels;
	
private:
	static UserSettings* s_pxInstance;
	UserSettings(const std::string& filename);
	
public:
	static void Initialize(const std::string& filename);
	static void Terminate();

	static UserSettings& GetInstance();
	
public:
	LevelSettings& GetLevels();
	LevelSetting& GetLevel(const std::string& name);

	void Save();

private:
	void Clear();
	
	bool Load(const std::string& filename);
	bool Load(TiXmlHandle& settings);

	bool Save(const std::string& filename);
};

#endif
