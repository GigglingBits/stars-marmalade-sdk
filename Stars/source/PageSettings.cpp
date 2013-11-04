#include <sstream>

#include "PageSettings.h"

#include "Debug.h"

PageSettings::PageSettings() :
	m_eSelectedWorld(eWorldIdEarth),
	m_iLevel(0),
	m_xNullColours(0x00000000, 0x00000000, 0x00000000, 0x00000000),
	m_xWorldColours(0xffd6834b, 0xffd6834b, 0xff320000, 0xff320000),
	m_xMarsColours(0xff0050ae, 0xff0050ae, 0x00000000, 0x00000000),
	m_xJupiterColours(0xff9d8a85, 0xff9d8a85, 0x00000000, 0x00000000) {
    UpdateKeys();
}

PageSettings::WorldId PageSettings::GetWorld() const {
    return m_eSelectedWorld;
}

void PageSettings::SetWorld(WorldId world) {
    m_eSelectedWorld = world;
    UpdateKeys();
}

int PageSettings::GetLevel() const {
    return m_iLevel;
}

void PageSettings::SetLevel(int level) {
    m_iLevel = level;
    UpdateKeys();
}

const std::string& PageSettings::GetWorldKey() {
    return m_sWorldKey;
}

const std::string& PageSettings::GetLevelKey() {
    return m_sLevelKey;
}

void PageSettings::UpdateKeys() {
    // world key
    m_sWorldKey = GetWorldKey(m_eSelectedWorld);
    
    // level key
    std::ostringstream oss;
    oss << m_sWorldKey << "_" << m_iLevel;
	m_sLevelKey = oss.str();
}

const PageSettings::Colours& PageSettings::GetWorldColours() {
	IW_CALLSTACK_SELF;
	
    switch (m_eSelectedWorld) {
        case eWorldIdEarth: { return m_xWorldColours; }
        case eWorldIdMars: { return m_xMarsColours; }
        case eWorldIdJupiter: { return m_xJupiterColours; }
        default:
        {
            IwAssertMsg(MYAPP, false, ("Invalid world selected: %i", m_eSelectedWorld));
            return m_xNullColours;
        }
    }
}

const std::string PageSettings::GetWorldKey(WorldId world) {
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
