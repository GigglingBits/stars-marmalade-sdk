#ifndef __LEVELITERATOR_H__
#define __LEVELITERATOR_H__

#include <string>

#define LEVELITERATOR_NO_LEVEL 0
#define LEVELITERATOR_MIN_LEVEL 1
#define LEVELITERATOR_MAX_LEVEL 18

#define LEVELITERATOR_MAX_LEVEL_COUNT (LEVELITERATOR_MAX_LEVEL - LEVELITERATOR_MIN_LEVEL)

class LevelIterator {
public:
	std::string GetLevelName(int level);

	int GetFirstLevel();
	int GetNextLevel(int level);
};

#endif
