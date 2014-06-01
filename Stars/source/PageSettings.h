#ifndef __PAGESETTINGS_H__
#define __PAGESETTINGS_H__

#include "LevelIterator.h"

class PageSettings {
private:
	LevelIterator::WorldId m_eSelectedWorld;
    int m_iSelectedLevel;

public:
	PageSettings();
    
    void SetWorld(LevelIterator::WorldId world);
	LevelIterator::WorldId GetWorld() const;

    void SetLevel(int level);
    int GetLevel() const;
};

#endif
