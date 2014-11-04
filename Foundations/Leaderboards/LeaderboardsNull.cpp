#include "LeaderboardsNull.h"

bool LeaderboardsNull::IsAvailable() {
	return true;
}

bool LeaderboardsNull::IsAuthenticated() {
	return true;
}

bool LeaderboardsNull::WaitForAuthentication(uint16 milliseconds) {
	return true;
}

void LeaderboardsNull::SaveScore(const std::string& leaderboardid, unsigned long score) {
}

bool LeaderboardsNull::ShowLeaderboard(const std::string& leaderboardid) {
	return false;
}
