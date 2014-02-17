#include "UserSettings.h"
#include "IwDebug.h"

UserSettings* UserSettings::s_pxInstance = NULL;

UserSettings::UserSettings(const std::string& filename) : m_sSettingsFileName(filename) {
	Load(m_sSettingsFileName);
}

void UserSettings::Initialize(const std::string& filename) {
	if (!s_pxInstance) {
		s_pxInstance = new UserSettings(filename);
	}
}

void UserSettings::Terminate() {
	if (s_pxInstance) {
		delete s_pxInstance;
		s_pxInstance = NULL;
	}
}

UserSettings& UserSettings::GetInstance() {
	IwAssertMsg(MYAPP, s_pxInstance, ("UserSettings instance not constructed. Call Initiailise() before getting the instance."));
	return *s_pxInstance;
}

UserSettings::LevelSettings& UserSettings::GetLevels() {
	return m_xLevels;
}

UserSettings::LevelSetting& UserSettings::GetLevel(const std::string& name) {
	return m_xLevels[name];
}

void UserSettings::Clear() {
	m_xLevels.clear();
}

bool UserSettings::Load(const std::string& filename) {
	Clear();

	TiXmlDocument doc(filename.c_str());
	if (!doc.LoadFile()) return false;
	TiXmlHandle hdoc(&doc);

	TiXmlElement* elem = hdoc.FirstChildElement(USER_SETTINGS_SETTINGS_TAG).Element();
	if (elem) {
		int fileformatversion = USER_SETTINGS_FILEFORMATVERSION_NULL;
		if (elem->Attribute(USER_SETTINGS_FILEFORMATVERSION_ATTR, &fileformatversion)) {
			if (fileformatversion == USER_SETTINGS_FILEFORMATVERSION) {
				TiXmlHandle settings(elem);
				return Load(settings);
			} else {
				IwAssertMsg(MYAPP, fileformatversion == USER_SETTINGS_FILEFORMATVERSION, ("Error loading file %s. The file format version number does not match: %i <> %i", filename.c_str(), fileformatversion, USER_SETTINGS_FILEFORMATVERSION));
			}
		} else {
			IwAssertMsg(MYAPP, false, ("Error loading file %s. The file format version number could not be found.", filename.c_str()));
		}
	} else {
		IwAssertMsg(MYAPP, false, ("Error loading file %s. The settings element could not be found.", filename.c_str()));
	}
	
	return false;
}

bool UserSettings::Load(TiXmlHandle& settings) {
	// load levels
	TiXmlElement* levelselem = settings.FirstChild(USER_SETTINGS_LEVELS_TAG).Element();
	for (levelselem; levelselem; levelselem = levelselem->NextSiblingElement(USER_SETTINGS_LEVELS_TAG)) {
		TiXmlElement* levelelem = levelselem->FirstChildElement(USER_SETTINGS_LEVEL_TAG);
		for (levelelem; levelelem; levelelem = levelelem->NextSiblingElement(USER_SETTINGS_LEVEL_TAG)) {
			if (const char* id = levelelem->Attribute (USER_SETTINGS_ID_ATTR)) {
				LevelSetting& level = m_xLevels[id];
				
				int playcount = 0;
				if (levelelem->Attribute(USER_SETTINGS_PLAYCOUNT_ATTR, &playcount)) {
					level.PlayCount = playcount;
				}
				
				int highscore = 0;
				if (levelelem->Attribute(USER_SETTINGS_HIGHSCORE_ATTR, &highscore)) {
					level.HighScore = highscore;
				}
				
				int stars = 0;
				if (levelelem->Attribute(USER_SETTINGS_STARS_ATTR, &stars)) {
					level.Stars = stars;
				}
			}
		}
	}
	
	return false;
}

void UserSettings::Save() {
	Save(m_sSettingsFileName);
}

bool UserSettings::Save(const std::string& filename) {
	TiXmlDocument doc;

 	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild( decl );
	
	TiXmlElement * root = new TiXmlElement(USER_SETTINGS_SETTINGS_TAG);
	doc.LinkEndChild( root );
	
	root->SetAttribute(USER_SETTINGS_FILEFORMATVERSION_ATTR, USER_SETTINGS_FILEFORMATVERSION);
	
	// block: levels
	{
		TiXmlElement* levels = new TiXmlElement(USER_SETTINGS_LEVELS_TAG);
		root->LinkEndChild(levels);
		
		for (LevelSettings::iterator i = m_xLevels.begin(); i != m_xLevels.end(); i++) {
			TiXmlElement* level = new TiXmlElement(USER_SETTINGS_LEVEL_TAG);
			levels->LinkEndChild(level);
			
			level->SetAttribute(USER_SETTINGS_ID_ATTR, i->first.c_str());
			level->SetAttribute(USER_SETTINGS_PLAYCOUNT_ATTR, i->second.PlayCount);
			level->SetAttribute(USER_SETTINGS_HIGHSCORE_ATTR, i->second.HighScore);
			level->SetAttribute(USER_SETTINGS_STARS_ATTR, i->second.Stars);
		}
	}
	
	return doc.SaveFile(filename.c_str());
}
