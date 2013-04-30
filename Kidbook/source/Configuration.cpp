#include <string>
#include "s3e.h"

#include "Configuration.h"

Configuration* Configuration::m_pxInstance = NULL;

Configuration::Configuration() {
	AccelerometerStarted = false;
	ShowStats = false;

	char tmpstr[S3E_CONFIG_STRING_MAX] = {'\0'};
	int tmpint = 0;
	if (S3E_RESULT_SUCCESS == s3eConfigGetInt("Kidbook", "showstats", &tmpint)) {
		ShowStats = tmpint == 1;
	} else {
		ShowStats = 0;
	}

	if (S3E_RESULT_SUCCESS == s3eConfigGetInt("Kidbook", "autorotate", &tmpint)) {
		AutoRotate = tmpint == 1;
	} else {
		AutoRotate = false;
	}

	if (S3E_RESULT_SUCCESS == s3eConfigGetInt("Kidbook", "exitbutton", &tmpint)) {
		ExitButton = tmpint == 1;
	} else {
		ExitButton = false;
	}

	if (S3E_RESULT_SUCCESS == s3eConfigGetInt("Kidbook", "helpbutton", &tmpint)) {
		HelpButton = tmpint == 1;
	} else {
		HelpButton = false;
	}

	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Kidbook", "language", tmpstr)) {
		Language = tmpstr;
	} else {
		Language = GetDeviceLanguage();
	}

	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Kidbook", "locale", tmpstr)) {
		Locale = tmpstr;
	} else {
		const char* pc = s3eDeviceGetString(S3E_DEVICE_LOCALE);
		if (pc) {
			Locale = pc;
		} else {
			Locale = "";
		}
	}

	if (Language.empty()) {
		EnableSpeech = false;
		EnableText = false;
	} else {
		if (S3E_RESULT_SUCCESS == s3eConfigGetInt("Kidbook", "enable-speech", &tmpint)) {
			EnableSpeech = tmpint == 1;
			EnableSpeech &= SpeechFolderExists();
		}
		if (S3E_RESULT_SUCCESS == s3eConfigGetInt("Kidbook", "enable-text", &tmpint)) {
			EnableText = tmpint == 1;
		}
	}

	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Kidbook", "pagemap-file", tmpstr)) {
		PageMapFile = tmpstr;
	} else {
		PageMapFile = "PageMap.xml";
	}

	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Kidbook", "help-file", tmpstr)) {
		HelpFile = tmpstr;
	} else {
		HelpFile = "help.txt";
	}

	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Kidbook", "anim-constant", tmpstr)) {
		AnimationConstant = (float)atof(tmpstr);
	} else {
		AnimationConstant = 0.8f;
	}
}

void Configuration::Initialize() {
	Configuration config = GetInstance();
}

void Configuration::UnInitialize() {
	if (m_pxInstance) {
		delete m_pxInstance;
		m_pxInstance = NULL;
	}
}

Configuration& Configuration::GetInstance() {
	if (!m_pxInstance) {
		m_pxInstance = new Configuration();
	}
	return *m_pxInstance;
}

bool Configuration::SpeechFolderExists() {
	return 1 == s3eFileGetFileInt((std::string("speech/").append(Language)).c_str(), S3E_FILE_ISDIR);
}

const char* Configuration::GetDeviceLanguage() {
	// mission: translate language id to ISO 639 format (except for Swiss German)
	s3eDeviceLanguage langid = (s3eDeviceLanguage)s3eDeviceGetInt(S3E_DEVICE_LANGUAGE);
	switch (langid) {
		case S3E_DEVICE_LANGUAGE_ENGLISH: return "en";
		case S3E_DEVICE_LANGUAGE_GERMAN: return "de";
		case S3E_DEVICE_LANGUAGE_DANISH: return "da";
		case S3E_DEVICE_LANGUAGE_SWEDISH: return "sv";
		//case S3E_DEVICE_LANGUAGE_SPANISH: return "es";
		//case S3E_DEVICE_LANGUAGE_FRENCH: return "fr";
	}
	return "";
}
