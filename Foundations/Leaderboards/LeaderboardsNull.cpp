#include "LeaderboardsNull.h"

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
