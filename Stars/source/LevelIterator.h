#ifndef __LEVELITERATOR_H__
#define __LEVELITERATOR_H__

#include <string>

#define LEVELITERATOR_NO_LEVEL 0
#define LEVELITERATOR_MIN_LEVEL_IN_WORLD 1
#define LEVELITERATOR_MAX_LEVEL_IM_WORLD 18

#define LEVELITERATOR_MAX_LEVEL_COUNT_IM_WORLD (LEVELITERATOR_MAX_LEVEL_IN_WORLD - LEVELITERATOR_MIN_LEVEL_IN_WORLD)

class LevelIterator {
public: 
	enum WorldId {
		eWorldIdEarth = 0,
		eWorldIdMars = 1,
		eWorldIdJupiter = 2,
        eWorldIdMax = 3
	};
	
public:
	std::string GetLevelName(WorldId world, int level);
	std::string GetWorldName(WorldId world);

	WorldId GetFirstWorld();
	WorldId GetNextWorld(WorldId world);
	WorldId GetPreviousWorld(WorldId world);

	int GetFirstLevelInWorld(WorldId world);
	int GetNextLevelInWorld(WorldId world, int level);
};

#endif
