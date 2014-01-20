#include "UserSettings.h"

UserSettings::LevelSettings& UserSettings::GetLevels() {
	return m_xLevels;
}

UserSettings::LevelSetting& UserSettings::GetLevel(const std::string& name) {
	return m_xLevels[name];
}

bool UserSettings::Load(const std::string& filename) {
	return false;
}

bool UserSettings::Save(const std::string& filename) {
	return false;
}
