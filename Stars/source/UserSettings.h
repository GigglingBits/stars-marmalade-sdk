#ifndef __USERSETTINGS_H__ 
#define __USERSETTINGS_H__ 

#include <string>
#include <map>

class UserSettings {
public:
	struct LevelSetting {
		int highscore;
		int stars;
		LevelSetting() {
			highscore = 0;
			stars = 0;
		}
	};

private:
	typedef std::map<std::string, LevelSetting> LevelSettings;
	LevelSettings m_xLevels;

public:
	LevelSettings& GetLevels();
	LevelSetting& GetLevel(const std::string& name);
	
	bool Load(const std::string& filename);
	bool Save(const std::string& filename);
};

#endif
