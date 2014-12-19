#include "LeaderboardsNull.h"

const std::string& LeaderboardsNull::GetMediaName() {
	static const std::string s("");
	return s;
}

bool LeaderboardsNull::IsAvailable() {
	return false;
}

bool LeaderboardsNull::IsAuthenticated() {
	return false;
}

bool LeaderboardsNull::WaitForAuthentication(uint16 milliseconds) {
	return false;
}

void LeaderboardsNull::SaveScore(const std::string& leaderboardid, unsigned long score) {
}

bool LeaderboardsNull::ShowLeaderboard(const std::string& leaderboardid) {
	return false;
}

void LeaderboardsNull::SaveAchievement(const std::string& achievementid, uint8 percent) {
}

bool LeaderboardsNull::ShowAchievements() {
	return false;
}
