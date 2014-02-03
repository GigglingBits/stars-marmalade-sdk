#ifndef __PAGESETTINGS_H__
#define __PAGESETTINGS_H__

#include "LevelIndexer.h"

class PageSettings {
public: 
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
	LevelIndexer::WorldId m_eSelectedWorld;
    int m_iSelectedLevel;
    
	Colours m_xNullColours;
	Colours m_xWorldColours;
	Colours m_xMarsColours;
	Colours m_xJupiterColours;
	
public:
	PageSettings();
    
    void SetWorld(LevelIndexer::WorldId world);
	LevelIndexer::WorldId GetWorld() const;

    void SetLevel(int level);
    int GetLevel() const;

    std::string GetWorldKey();
    std::string GetLevelKey();

	const Colours& GetWorldColours();
};

#endif
