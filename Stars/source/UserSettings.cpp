#include "UserSettings.h"
#include "Debug.h"
#include "IwDebug.h"

#include "s3eCrypto.h"
#include "Analytics.h"

#include <vector>
#include <sstream>
#include <iomanip>

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
	IW_CALLSTACK_SELF;
	
	IwAssertMsg(MYAPP, s_pxInstance, ("UserSettings instance not constructed. Call Initiailise() before getting the instance."));
	return *s_pxInstance;
}

UserSettings::LevelSettings& UserSettings::GetLevels() {
	return m_xLevels;
}

UserSettings::LevelSetting& UserSettings::GetLevel(const std::string& name) {
	return m_xLevels[name];
}

unsigned long UserSettings::GetTotalScore() {
	unsigned long totalscore = 0;
	UserSettings::LevelSettings::iterator it;
	for (it = m_xLevels.begin(); it != m_xLevels.end(); it++) {
		totalscore += (*it).second.HighScore;
	}
	return totalscore;
}

void UserSettings::Clear() {
	m_xLevels.clear();
}

bool UserSettings::Load(const std::string& filename) {
	IW_CALLSTACK_SELF;

	Clear();

	TiXmlDocument doc(filename.c_str());
	if (!doc.LoadFile()) return false;
	TiXmlHandle hdoc(&doc);

	TiXmlElement* elem = hdoc.FirstChildElement(USER_SETTINGS_SETTINGS_TAG).Element();
	if (elem) {
		int fileformatversion = USER_SETTINGS_FILEFORMATVERSION_NULL;
		if (elem->Attribute(USER_SETTINGS_FILEFORMATVERSION_ATTR, &fileformatversion)) {
			if (fileformatversion >= 1 && fileformatversion <= USER_SETTINGS_FILEFORMATVERSION) {
				TiXmlHandle settings(elem);
				return Load(settings, fileformatversion);
			} else {
				IwAssertMsg(MYAPP, fileformatversion >= 1 && fileformatversion <= USER_SETTINGS_FILEFORMATVERSION, ("Error loading file %s. The file format version number %i is not valid. The expected number WAS %i. All settings will be discarded.", filename.c_str(), fileformatversion, USER_SETTINGS_FILEFORMATVERSION));
			}
		} else {
			IwAssertMsg(MYAPP, false, ("Error loading file %s. The file format version number could not be found.", filename.c_str()));
		}
	} else {
		IwAssertMsg(MYAPP, false, ("Error loading file %s. The settings element could not be found.", filename.c_str()));
	}
	
	return false;
}

bool UserSettings::Load(TiXmlHandle& settings, int fileformatversion) {
	LevelSettings levels;
	
	std::string checksum;
	
	// load levels
	TiXmlElement* levelselem = settings.FirstChild(USER_SETTINGS_LEVELS_TAG).Element();
	for (; levelselem; levelselem = levelselem->NextSiblingElement(USER_SETTINGS_LEVELS_TAG)) {
		TiXmlElement* levelelem = levelselem->FirstChildElement(USER_SETTINGS_LEVEL_TAG);
		for (; levelelem; levelelem = levelelem->NextSiblingElement(USER_SETTINGS_LEVEL_TAG)) {
			if (const char* id = levelelem->Attribute(USER_SETTINGS_ID_ATTR)) {
				LevelSetting& level = levels[id];

				// file format version 1 and later
				if (fileformatversion > 0) {
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

				// file format version 2 and later
				if (fileformatversion > 1) {
					int birdkills = 0;
					if (levelelem->Attribute(USER_SETTINGS_BIRDKILLS_ATTR, &birdkills)) {
						level.BirdKills = birdkills;
					}
					
					int fulllifecompletions = 0;
					if (levelelem->Attribute(USER_SETTINGS_FULLLIFECOMPLETIONS_ATTR, &fulllifecompletions)) {
						level.FullLifeCompletions = fulllifecompletions;
					}
					
					int buffmagnets = 0;
					if (levelelem->Attribute(USER_SETTINGS_BUFFMAGNETS_ATTR, &buffmagnets)) {
						level.BuffMagentsUsed = buffmagnets;
					}
					
					int buffshields = 0;
					if (levelelem->Attribute(USER_SETTINGS_BUFFSHIELDS_ATTR, &buffshields)) {
						level.BuffShieldsUsed = buffshields;
					}
					
					int buffshots = 0;
					if (levelelem->Attribute(USER_SETTINGS_BUFFSHOTS_ATTR, &buffshots)) {
						level.BuffShotsUsed = buffshots;
					}
				}
				
			}
		}
		
		const char* pc;
		checksum = (pc = levelselem->Attribute(USER_SETTINGS_CHECKSUM_ATTR)) ? pc : "";
	}
	
	// validate checksum
	std::string data = DataToString(levels, fileformatversion);
	if (!checksum.compare(GetHash(data))) {
		m_xLevels = levels;
		return true;
	} else if (!checksum.compare(USER_SETTINGS_NULL_CHECKSUM)) {
		Analytics::GetInstance().Log("settings: null hash found -> hash will be added");
		m_xLevels = levels;
		return true;
	} else if (fileformatversion < 2 && checksum.empty()) {
		Analytics::GetInstance().Log("settings: no hash found -> hash will be added");
		m_xLevels = levels;
		return true;
	} else {
		Analytics::GetInstance().Log("settings: hash mismatch -> settings lost");
		IwAssertMsg(MYAPP, false, ("The settings file has an invalid checksum. All settings will be discarded. Has the settings file been tampered with?"));
		return false;
	}
}

void UserSettings::Save() {
	Save(m_sSettingsFileName);
}

bool UserSettings::Save(const std::string& filename) {
	TiXmlDocument doc;

 	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild( decl );
	
	TiXmlElement * root = new TiXmlElement(USER_SETTINGS_SETTINGS_TAG);
	doc.LinkEndChild(root);
	
	root->SetAttribute(USER_SETTINGS_FILEFORMATVERSION_ATTR, USER_SETTINGS_FILEFORMATVERSION);
	
	// block: levels
	{
		TiXmlElement* levels = new TiXmlElement(USER_SETTINGS_LEVELS_TAG);
		root->LinkEndChild(levels);
		
		std::string data = DataToString(m_xLevels, USER_SETTINGS_FILEFORMATVERSION);
		levels->SetAttribute(USER_SETTINGS_CHECKSUM_ATTR, GetHash(data).c_str());

		for (LevelSettings::iterator i = m_xLevels.begin(); i != m_xLevels.end(); i++) {
			TiXmlElement* level = new TiXmlElement(USER_SETTINGS_LEVEL_TAG);
			levels->LinkEndChild(level);

			level->SetAttribute(USER_SETTINGS_ID_ATTR, i->first.c_str());
			level->SetAttribute(USER_SETTINGS_PLAYCOUNT_ATTR, i->second.PlayCount);
			level->SetAttribute(USER_SETTINGS_HIGHSCORE_ATTR, i->second.HighScore);
			level->SetAttribute(USER_SETTINGS_STARS_ATTR, i->second.Stars);
			level->SetAttribute(USER_SETTINGS_BIRDKILLS_ATTR, i->second.BirdKills);
			level->SetAttribute(USER_SETTINGS_FULLLIFECOMPLETIONS_ATTR, i->second.FullLifeCompletions);
			level->SetAttribute(USER_SETTINGS_BUFFMAGNETS_ATTR, i->second.BuffMagentsUsed);
			level->SetAttribute(USER_SETTINGS_BUFFSHIELDS_ATTR, i->second.BuffShieldsUsed);
			level->SetAttribute(USER_SETTINGS_BUFFSHOTS_ATTR, i->second.BuffShotsUsed);
		}
	}
	
	return doc.SaveFile(filename.c_str());
}

std::string UserSettings::GetHash(const std::string& data) {
	IW_CALLSTACK_SELF;

	const uint8 sha1buflen = 20;
	uint8 sha1buf[sha1buflen];
	
	if (S3E_RESULT_SUCCESS == s3eCryptoSha1((uint8*)data.c_str(), data.length(), sha1buf)) {
		std::string hash(BytesToHex(sha1buf, sha1buflen));
		IwAssert(MYAPP, hash.length() > 0);
		return hash;
	} else {
		IwAssertMsg(MYAPP, false, ("Cannot create checksum: %s", s3eCryptoGetErrorString()));
		Analytics::GetInstance().Log("settings: cannot calculate hash -> null hash will be added");
		return USER_SETTINGS_NULL_CHECKSUM;
	}
}

std::string UserSettings::DataToString(const LevelSettings& settings, int fileformatversion) {
	std::ostringstream ss;
	ss << std::uppercase;
	
	// std::map is always sorted by key; so, this will always produce the same result
	for (LevelSettings::const_iterator it = settings.begin(); it != settings.end(); it++) {
		ss << it->first;
		
		// file format version 1 and later
		if (fileformatversion > 0) {
			ss << "," << it->second.Stars;
			ss << "," << it->second.HighScore;
			ss << "," << it->second.PlayCount;
		}
		
		// file format version 2 and later
		if (fileformatversion > 1) {
			ss << "," << it->second.BirdKills;
			ss << "," << it->second.FullLifeCompletions;
			ss << "," << it->second.BuffMagentsUsed;
			ss << "," << it->second.BuffShieldsUsed;
			ss << "," << it->second.BuffShotsUsed;
		}
		ss << std::endl;
	}
	return ss.str();
}

std::string UserSettings::BytesToHex(uint8* data, uint8 datalen) {
	const char lookup[] = "0123456789abcdef";
	std::string s;
	for (int i = 0; i < datalen; i++) {
		s += lookup[data[i] >> 4];
		s += lookup[data[i] & 0x0f];
	}
	return s;
/*
	std::ostringstream ss;
	ss << std::hex << std::uppercase << std::setfill('0');
	
	for (int i = 0; i < datalen; i++) {
		unsigned char c = data[i];
		ss << std::setw(2) << c;
	}
	
	std::string hexstr = ss.str();
	return hexstr;
 */
}
