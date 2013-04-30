#ifndef __LEVELCOMPLETIONINFO_H__
#define __LEVELCOMPLETIONINFO_H__

#include <string>
#include <map>

class LevelCompletionInfo {
private:
	struct HeroState{
		bool rescued;
		float health;
	};

	typedef std::map<std::string, HeroState> HeroStateMap;
	HeroStateMap m_xHeroStates;

public:
	LevelCompletionInfo();

	void SetHeroState(const std::string& id, bool rescued, float health);

	int GetRescuedCount();
	int GetKilledCount();
};

#endif
