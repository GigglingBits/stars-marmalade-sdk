#include "s3e.h"

#include "Configuration.h"

Configuration* Configuration::s_pxInstance = NULL;

Configuration::Configuration() {

	Read(ShowStats, "showstats", 0);
	Read(ShowShapes, "showshapes", 0);
	Read(UnlockAll, "unlockall", 0);
	
	Read(ResourceHeapSize, "resourceheapsize", 0);
	
	Read(StarBirthDelay, "starbirthdelay", 0);

	Read(BodiesFile, "bodies", "bodies.xml");
	Read(LevelsFile, "levels", "levels.xml");

	Read(IntroMovie, "intromovie", "movies/intro.mp3");
	Read(MenuSong, "menusong", "music/menusong.mp3");
	Read(LevelSong, "levelsong", "music/levelsong.mp3");

	Read(AppFont, "appfont", "fonts/app.ttf");
	Read(SysFont, "sysfont", "fonts/sys.ttf");
	
	Read(HttpBodiesFile, "httpbodies", "");
	Read(HttpLevelsFile, "httplevels", "");

	Read(FacebookPage, "facebookpage", "");

	Read(MaxVisibleWorldSize, "maxvisibleworldsize", 20.0f);
	Read(WorldMargin, "worldmargin", 5.0f);
	Read(Gravity, "gravity", -9.81f);

	Read(BuffSpeed, "buffspeed", 0.5f);
	Read(BuffShieldDuration, "buffshieldduration", 1000);
	Read(BuffMagnetDuration, "buffmagnetduration", 1000);
	Read(BuffShootDuration, "buffshootduration", 1000);
	Read(BuffShootCount, "buffshootcount", 1);
	
	Read(PathSpeed, "pathspeed", 2.0f);
	Read(PathMaxLength, "pathmaxlength", 10.0f);

	Read(SettingsFile, "settingsfile", "settings.xml");
	
	Read(FlurryKey, "flurrykey", "");
	Read(LeaderboardKey, "leaderboardkey", "");
	
	Read(AchievementBirdKillsKey, "achibirdkillskey", "");
	Read(AchievementFullLifeCompletionsKey, "achifulllifekey", "");
	Read(AchievementBuffMagnetsKey, "achibuffmagnetskey", "");
	Read(AchievementBuffShieldsKey, "achibuffshieldskey", "");
	Read(AchievementBuffShotsKey, "achibuffshotskey", "");

	Read(AchievementBirdKillsValue, "achibirdkillsval", 1000);
	Read(AchievementFullLifeCompletionsValue, "achifulllifeval", 1000);
	Read(AchievementBuffMagnetsValue, "achibuffmagnetsval", 1000);
	Read(AchievementBuffShieldsValue, "achibuffshieldsval", 1000);
	Read(AchievementBuffShotsValue, "achibuffshotsval", 1000);
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

bool Configuration::Read(float& val, const std::string& key, float def) {
	static char tmpstr[S3E_CONFIG_STRING_MAX] = {'\0'};
	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", key.c_str(), tmpstr)) {
		val = atof(tmpstr);
		return true;
	}
	val = def;
	return false;
}

bool Configuration::Read(int& val, const std::string& key, int def) {
	int tmpint;
	if (S3E_RESULT_SUCCESS == s3eConfigGetInt("Game", key.c_str(), &tmpint)) {
		val = tmpint;
		return true;
	}
	val = def;
	return false;
}

bool Configuration::Read(bool& val, const std::string& key, bool def) {
	int tmpint;
	if (S3E_RESULT_SUCCESS == s3eConfigGetInt("Game", key.c_str(), &tmpint)) {
		val = tmpint == 1;
		return true;
	}
	val = def;
	return false;
}

bool Configuration::Read(std::string& val, const std::string& key, std::string def) {
	static char tmpstr[S3E_CONFIG_STRING_MAX] = {'\0'};
	if (S3E_RESULT_SUCCESS == s3eConfigGetString("Game", key.c_str(), tmpstr)) {
		val = tmpstr;
		return true;
	}
	val = def;
	return false;
}

