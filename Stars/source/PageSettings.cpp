#include <sstream>

#include "PageSettings.h"

#include "IwDebug.h"
#include "Debug.h"

PageSettings::PageSettings() : m_iSelectedLevel(0) {
}

int PageSettings::GetLevel() const {
    return m_iSelectedLevel;
}

void PageSettings::SetLevel(int level) {
    m_iSelectedLevel = level;
}
