#ifndef __PAGESETTINGS_H__
#define __PAGESETTINGS_H__

#include "Window.h"

class PageSettings {
public: 
	enum WorldId {
		eWorldIdEarth = 0,
		eWorldIdMars = 1,
		eWorldIdJupiter = 2,
        eWorldIdMax = 3
	};
    
private:
	WorldId m_eSelectedWorld;
    int m_iLevel;
    
    std::string m_sWorldKey;
    std::string m_sLevelKey;

public:
	PageSettings();
    
    void SetWorld(WorldId world);
    WorldId GetWorld() const;

    void SetLevel(int level);
    int GetLevel() const;

    const std::string& GetWorldKey();
    const std::string& GetLevelKey();

    static const std::string GetWorldKey(WorldId world);

private:
    void UpdateKeys();
};

#endif
