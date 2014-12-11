#ifndef __USERSETTINGS_H__ 
#define __USERSETTINGS_H__ 

#include <string>
#include <map>

#include "tinyxml.h"

#define USER_SETTINGS_FILEFORMATVERSION_NULL 0

#define USER_SETTINGS_FILEFORMATVERSION_ATTR "fileformatversion"
#define USER_SETTINGS_FILEFORMATVERSION 2

#define USER_SETTINGS_SETTINGS_TAG "settings"
#define USER_SETTINGS_LEVELS_TAG "levels"
#define USER_SETTINGS_LEVEL_TAG "level"

#define USER_SETTINGS_ID_ATTR "id"
#define USER_SETTINGS_PLAYCOUNT_ATTR "playcount"
#define USER_SETTINGS_HIGHSCORE_ATTR "highscore"
#define USER_SETTINGS_STARS_ATTR "stars"
#define USER_SETTINGS_BIRDKILLS_ATTR "birdkills"
#define USER_SETTINGS_FULLLIFECOMPLETIONS_ATTR "fulllifecompletions"
#define USER_SETTINGS_BUFFMAGNETS_ATTR "buffmagnets"
#define USER_SETTINGS_BUFFSHIELDS_ATTR "buffshields"
#define USER_SETTINGS_BUFFSHOTS_ATTR "buffshots"

#define USER_SETTINGS_CHECKSUM_ATTR "checksum"
#define USER_SETTINGS_NULL_CHECKSUM "********"

class UserSettings {
public:
	struct LevelSetting {
		int PlayCount;
		
		int HighScore;
		int Stars;
		
		int BirdKills;
		int FullLifeCompletions;
		int BuffMagnetsUsed;
		int BuffShieldsUsed;
		int BuffShotsUsed;
		
		LevelSetting() {
			PlayCount = 0;
			HighScore = 0;
			Stars = 0;

			BirdKills = 0;
			FullLifeCompletions = 0;
			BuffMagnetsUsed = 0;
			BuffShieldsUsed = 0;
			BuffShotsUsed = 0;
		}
	};

	typedef std::map<std::string, LevelSetting> LevelSettings;

private:
	std::string m_sSettingsFileName;
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

	unsigned long GetTotalScore();
	
	void Save();

private:
	void Clear();
	
	bool Load(const std::string& filename);
	bool Load(TiXmlHandle& settings, int fileformatversion);

	bool Save(const std::string& filename);
	
	std::string DataToString(const LevelSettings& settings, int fileformatversion);

	std::string GetHash(const std::string& data);
	std::string BytesToHex(uint8* data, uint8 datalen);
};

#endif
