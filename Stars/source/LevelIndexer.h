#ifndef __LEVELINDEXER_H__
#define __LEVELINDEXER_H__

#include <string>

class LevelIndexer {
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
};

#endif
