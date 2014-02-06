#include "LevelIterator.h"
#include "FactoryManager.h"

#include <sstream>

#include "IwDebug.h"
#include "Debug.h"

std::string LevelIterator::GetLevelName(WorldId world, int level) {
	IW_CALLSTACK_SELF;

    std::ostringstream oss;
    oss << GetWorldName(world) << "_" << level;
	return oss.str();
}

std::string LevelIterator::GetWorldName(WorldId world) {
	IW_CALLSTACK_SELF;
	
    switch (world) {
        case eWorldIdEarth: { return "earth"; }
        case eWorldIdMars: { return "mars"; }
        case eWorldIdJupiter: { return "jupiter"; }
        default:
        {
            IwAssertMsg(MYAPP, false, ("Invalid world selected: %i", world));
            return "earth";
        }
    }
}

LevelIterator::WorldId LevelIterator::GetFirstWorld() {
	return eWorldIdEarth;
}

LevelIterator::WorldId LevelIterator::GetNextWorld(WorldId world) {
	return (WorldId)((world + 1) % eWorldIdMax);
}

LevelIterator::WorldId LevelIterator::GetPreviousWorld(WorldId world) {
	return (WorldId)((world + eWorldIdMax - 1) % eWorldIdMax);
}

int LevelIterator::GetFirstLevelInWorld(WorldId world) {
	return GetNextLevelInWorld(world, LEVELITERATOR_MIN_LEVEL_IN_WORLD - 1);
}

int LevelIterator::GetNextLevelInWorld(WorldId world, int level) {
	for (int cursor = level + 1; cursor <= LEVELITERATOR_MAX_LEVEL_IM_WORLD; cursor++) {
		bool exists = FactoryManager::GetLevelFactory().ConfigExists(GetLevelName(world, cursor));
		if (exists) {
			return cursor;
		}
	}
	return LEVELITERATOR_NO_LEVEL;
}

