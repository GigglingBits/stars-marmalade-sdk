#include "LevelIterator.h"
#include "FactoryManager.h"

#include <sstream>

#include "IwDebug.h"
#include "Debug.h"

std::string LevelIterator::GetLevelName(int level) {
	IW_CALLSTACK_SELF;

    std::ostringstream oss;
    oss << "earth_" << level;
	return oss.str();
}

int LevelIterator::GetFirstLevel() {
	return GetNextLevel(LEVELITERATOR_MIN_LEVEL - 1);
}

int LevelIterator::GetNextLevel(int level) {
	for (int cursor = level + 1; cursor <= LEVELITERATOR_MAX_LEVEL; cursor++) {
		bool exists = FactoryManager::GetLevelFactory().ConfigExists(GetLevelName(cursor));
		if (exists) {
			return cursor;
		}
	}
	return LEVELITERATOR_NO_LEVEL;
}

