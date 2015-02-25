#ifndef __PAGESETTINGS_H__
#define __PAGESETTINGS_H__

#include "LevelIterator.h"

class PageSettings {
private:
    int m_iSelectedLevel;

public:
	PageSettings();
    
    void SetLevel(int level);
    int GetLevel() const;
};

#endif
