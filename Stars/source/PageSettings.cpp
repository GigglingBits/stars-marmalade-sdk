#include <sstream>

#include "PageSettings.h"

PageSettings::PageSettings() : m_eSelectedWorld(eWorldIdEarth), m_iLevel(0) {
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

const std::string PageSettings::GetWorldKey(WorldId world) {
    switch (world) {
        case eWorldIdEarth: { return "earth"; }
        case eWorldIdSky: { return "sky"; }
        case eWorldIdMars: { return "mars"; }
        case eWorldIdJupiter: { return "jupiter"; }
        default:
        {
            IwAssertMsg(MYAPP, false, ("Invalid world selected: %i", world));
            return "earth";
        }
    }
}
