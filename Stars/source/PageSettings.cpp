#include <sstream>

#include "PageSettings.h"

#include "IwDebug.h"
#include "Debug.h"

PageSettings::PageSettings() :
m_eSelectedWorld(LevelIterator::eWorldIdEarth),
	m_iSelectedLevel(0),
	m_xNullColours(0x00000000, 0x00000000, 0x00000000, 0x00000000),
	m_xWorldColours(0xffd6834b, 0xffd6834b, 0xff320000, 0xff320000),
	m_xMarsColours(0xff0050ae, 0xff0050ae, 0x00000000, 0x00000000),
	m_xJupiterColours(0xff9d8a85, 0xff9d8a85, 0x00000000, 0x00000000) {
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

const PageSettings::Colours& PageSettings::GetWorldColours() {
	IW_CALLSTACK_SELF;
	
    switch (m_eSelectedWorld) {
        case LevelIterator::eWorldIdEarth: { return m_xWorldColours; }
        case LevelIterator::eWorldIdMars: { return m_xMarsColours; }
        case LevelIterator::eWorldIdJupiter: { return m_xJupiterColours; }
        default:
        {
            IwAssertMsg(MYAPP, false, ("Invalid world selected: %i", m_eSelectedWorld));
            return m_xNullColours;
        }
    }
}
