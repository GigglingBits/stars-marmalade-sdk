#include "s3e.h"

#include "Configuration.h"

Configuration* Configuration::s_pxInstance = NULL;

Configuration::Configuration() {
	char tmpstr[S3E_CONFIG_STRING_MAX] = {'\0'};
	int tmpint = 0;
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetInt("Game", "showstats", &tmpint)) {
		ShowStats = tmpint == 1;
	} else {
		ShowStats = 0;
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetInt("Game", "showshapes", &tmpint)) {
		ShowShapes = tmpint == 1;
	} else {
		ShowShapes = 0;
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetInt("Game", "unlockall", &tmpint)) {
		UnlockAll = tmpint == 1;
	} else {
		UnlockAll = 0;
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetInt("Game", "resourceheapsize", &tmpint)) {
		ResourceHeapSize = tmpint;
	} else {
		ResourceHeapSize = 0;
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetInt("Game", "starbirthdelay", &tmpint)) {
		StarBirthDelay = tmpint;
	} else {
		StarBirthDelay = 0;
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "bodies", tmpstr)) {
		BodiesFile = tmpstr;
	} else {
		BodiesFile = "bodies.xml";
	}

	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "levels", tmpstr)) {
		LevelsFile = tmpstr;
	} else {
		LevelsFile = "levels.xml";
	}

	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "intromovie", tmpstr)) {
		IntroMovie = tmpstr;
	} else {
		IntroMovie = "music/introsong.mp3";
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "menusong", tmpstr)) {
		MenuSong = tmpstr;
	} else {
		MenuSong = "music/introsong.mp3";
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "levelsong", tmpstr)) {
		LevelSong = tmpstr;
	} else {
		LevelSong = "music/levelsong.mp3";
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "appfont", tmpstr)) {
		AppFont = tmpstr;
	} else {
		AppFont = "fonts/app.ttf";
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "sysfont", tmpstr)) {
		SysFont = tmpstr;
	} else {
		SysFont = "fonts/sys.ttf";
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "httpbodies", tmpstr)) {
		HttpBodiesFile = tmpstr;
	} else {
		HttpBodiesFile = "";
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "facebookpage", tmpstr)) {
		FacebookPage = tmpstr;
	} else {
		FacebookPage = "";
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "httplevels", tmpstr)) {
		HttpLevelsFile = tmpstr;
	} else {
		HttpLevelsFile = "";
	}

	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "maxvisibleworldsize", tmpstr)) {
		MaxVisibleWorldSize = atof(tmpstr);
	} else {
		MaxVisibleWorldSize = 20.0f;
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "worldmargin", tmpstr)) {
		WorldMargin = atof(tmpstr);
	} else {
		WorldMargin = 5.0f;
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "gravity", tmpstr)) {
		Gravity = atof(tmpstr);
	} else {
		Gravity = -9.81f;
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "buffspeed", tmpstr)) {
		BuffSpeed = atof(tmpstr);
	} else {
		BuffSpeed = 0.5f;
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "buffshieldduration", tmpstr)) {
		BuffShieldDuration = atoi(tmpstr);
	} else {
		BuffShieldDuration = 1000;
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "pathspeed", tmpstr)) {
		PathSpeed = atof(tmpstr);
	} else {
		PathSpeed = 2.0f;
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "pathmaxlength", tmpstr)) {
		PathMaxLength = atof(tmpstr);
	} else {
		PathMaxLength = 10.0f;
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "settingsfile", tmpstr)) {
		SettingsFile = tmpstr;
	} else {
		SettingsFile = "settings.xml";
	}
	
	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", "flurrykey", tmpstr)) {
		FlurryKey = tmpstr;
	} else {
		FlurryKey = "";
	}
}

void Configuration::Initialize() {
	GetInstance();
}

void Configuration::Terminate() {
	if (s_pxInstance) {
		delete s_pxInstance;
		s_pxInstance = NULL;
	}
}

Configuration& Configuration::GetInstance() {
	if (!s_pxInstance) {
		s_pxInstance = new Configuration();
	}
	return *s_pxInstance;
}

