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
    
	struct Colours {
		uint32 LowerLeft;
		uint32 LowerRight;
		uint32 UpperRight;
		uint32 UpperLeft;
		Colours(uint32 ll, uint32 lr, uint32 ur, uint32 ul) {
			LowerLeft = ll; LowerRight = lr;
			UpperRight = ur; UpperLeft = ul;
		}
	};
	
private:
	WorldId m_eSelectedWorld;
    int m_iLevel;
    
    std::string m_sWorldKey;
    std::string m_sLevelKey;

	Colours m_xNullColours;
	Colours m_xWorldColours;
	Colours m_xMarsColours;
	Colours m_xJupiterColours;
	
public:
	PageSettings();
    
    void SetWorld(WorldId world);
    WorldId GetWorld() const;

    void SetLevel(int level);
    int GetLevel() const;

    const std::string& GetWorldKey();
    const std::string& GetLevelKey();

	const Colours& GetWorldColours();
	
    static const std::string GetWorldKey(WorldId world);

private:
    void UpdateKeys();
};

#endif
