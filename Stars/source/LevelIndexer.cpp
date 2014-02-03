#include "LevelIndexer.h"

#include <sstream>

#include "IwDebug.h"
#include "Debug.h"

std::string LevelIndexer::GetLevelName(WorldId world, int level) {
	IW_CALLSTACK_SELF;

    std::ostringstream oss;
    oss << GetWorldName(world) << "_" << level;
	return oss.str();
}

std::string LevelIndexer::GetWorldName(WorldId world) {
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
