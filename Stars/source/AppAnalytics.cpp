#include <sstream>

#include "AppAnalytics.h"
#include "Analytics.h"
#include "Debug.h"

void AppAnalytics::RegisterTitleScreenOpened() {
	Analytics::GetInstance().Log("Title screen opened");
}

void AppAnalytics::RegisterWorldMenuOpened() {
	Analytics::GetInstance().Log("World menu opened");
}

void AppAnalytics::RegisterLevelMenuOpened(const std::string& worldid) {
	std::ostringstream oss;
	oss << "Level menu opened: " << worldid;
	Analytics::GetInstance().Log(oss.str());
}

void AppAnalytics::RegisterLevelStarted(const std::string& levelid) {
	std::ostringstream oss;
	oss << "Level started: " << levelid;
	Analytics::GetInstance().Log(oss.str());
}

void AppAnalytics::RegisterLevelCompleted(const std::string& id, float dustamount, int pathsstarted, bool achieved) {
	std::ostringstream oss;
	oss << "Level completed: " << id;
	std::string message(oss.str());
	
	oss.clear(); oss.str("");
	oss << dustamount;
	std::string dust(oss.str());
	
	oss.clear(); oss.str("");
	oss << pathsstarted;
	std::string paths(oss.str());
	
	Analytics::Params params;
	params["cleared"] = achieved ? "yes" : "no";
	params["dust amount"] = dust;
	params["#paths"] = paths;
	Analytics::GetInstance().Log(message, params);
}

void AppAnalytics::RegisterLevelCancelled(const std::string& levelid) {
	std::ostringstream oss;
	oss << "Level cancelled: " << levelid;
	Analytics::GetInstance().Log(oss.str());
}

void AppAnalytics::RegisterFacebookOpened() {
	Analytics::GetInstance().Log("Facebook opened");
}
