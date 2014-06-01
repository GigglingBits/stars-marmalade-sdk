#include <sstream>

#include "PageSettings.h"

#include "IwDebug.h"
#include "Debug.h"

PageSettings::PageSettings() :
m_eSelectedWorld(LevelIterator::eWorldIdEarth), m_iSelectedLevel(0) {
}

LevelIterator::WorldId PageSettings::GetWorld() const {
    return m_eSelectedWorld;
}

void PageSettings::SetWorld(LevelIterator::WorldId world) {
    m_eSelectedWorld = world;
}

int PageSettings::GetLevel() const {
    return m_iSelectedLevel;
}

void PageSettings::SetLevel(int level) {
    m_iSelectedLevel = level;
}
