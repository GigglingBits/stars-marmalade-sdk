#include "LevelCompletionInfo.h"
#include "Debug.h"

LevelCompletionInfo::LevelCompletionInfo() {
}

void LevelCompletionInfo::SetHeroState(const std::string& id, bool rescued, float health) {
	HeroState& state = m_xHeroStates[id];
	state.rescued = rescued;
	state.health = health;
}

int LevelCompletionInfo::GetRescuedCount() {
	int count = 0;
	for (HeroStateMap::iterator it = m_xHeroStates.begin(); it != m_xHeroStates.end(); it++) {
		count += it->second.rescued;
	}
	return count;
}

int LevelCompletionInfo::GetKilledCount() {
	int count = 0;
	for (HeroStateMap::iterator it = m_xHeroStates.begin(); it != m_xHeroStates.end(); it++) {
		count += !it->second.rescued;
	}
	return count;
}
