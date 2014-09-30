#include "UserSettings.h"
#include "Debug.h"
#include "IwDebug.h"

#include "s3eCrypto.h"

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
			if (fileformatversion == USER_SETTINGS_FILEFORMATVERSION) {
				TiXmlHandle settings(elem);
				return Load(settings);
			} else {
				IwAssertMsg(MYAPP, fileformatversion == USER_SETTINGS_FILEFORMATVERSION, ("Error loading file %s. The file format version number does not match: %i <> %i. All settings will be lost.", filename.c_str(), fileformatversion, USER_SETTINGS_FILEFORMATVERSION));
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
	LevelSettings levels;
	
	std::string checksum;
	
	// load levels
	TiXmlElement* levelselem = settings.FirstChild(USER_SETTINGS_LEVELS_TAG).Element();
	for (; levelselem; levelselem = levelselem->NextSiblingElement(USER_SETTINGS_LEVELS_TAG)) {
		TiXmlElement* levelelem = levelselem->FirstChildElement(USER_SETTINGS_LEVEL_TAG);
		for (; levelelem; levelelem = levelelem->NextSiblingElement(USER_SETTINGS_LEVEL_TAG)) {
			if (const char* id = levelelem->Attribute(USER_SETTINGS_ID_ATTR)) {
				LevelSetting& level = levels[id];
				
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
		
		const char* pc;
		checksum = (pc = levelselem->Attribute(USER_SETTINGS_CHECKSUM_ATTR)) ? pc : "";
	}
	
	// validate checksum
	if (!checksum.compare(GetHash(levels)) ||
		!checksum.compare(USER_SETTINGS_NULL_CHECKSUM) ||
		!checksum.compare("")) {
		// todo: empty string should no longer be allowed once
		//       all user have been converted
		m_xLevels = levels;
		return true;
	} else {
		IwAssertMsg(MYAPP, false, ("The settings file has an invalid checksum. All settings will be discarded. Has the settings fil ebeen tampered with?"));
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
		levels->SetAttribute(USER_SETTINGS_CHECKSUM_ATTR, GetHash(m_xLevels).c_str());

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

std::string UserSettings::GetHash(const LevelSettings settings) {
	IW_CALLSTACK_SELF;

	std::string data = DataToString(settings);

	const uint8 sha1buflen = 20;
	uint8 sha1buf[sha1buflen];
	
	if (S3E_RESULT_SUCCESS == s3eCryptoSha1((uint8*)data.c_str(), data.length(), sha1buf)) {
		std::string hash(BytesToHex(sha1buf, sha1buflen));
		IwAssert(MYAPP, hash.length() > 0);
		return hash;
	} else {
		IwAssertMsg(MYAPP, false, ("Cannot create checksum: %s", s3eCryptoGetErrorString()));
		return USER_SETTINGS_NULL_CHECKSUM;
	}
}

std::string UserSettings::DataToString(const LevelSettings settings) {
	std::ostringstream ss;
	ss << std::uppercase;
	
	// std::map is always sorted by key; so, this will always produce the same
	for (LevelSettings::const_iterator it = settings.begin(); it != settings.end(); it++) {
		ss << it->first << ",";
		ss << it->second.Stars << ",";
		ss << it->second.HighScore << ",";
		ss << it->second.PlayCount << std::endl;
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
