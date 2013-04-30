#ifndef __LEVELFACTORY_H__
#define __LEVELFACTORY_H__

#include "FactoryBase.h"
#include "LevelTemplate.h"
#include "Level.h"

class LevelFactory : public FactoryBase<LevelTemplate, Level> {
public:
	LevelFactory();

public:
	virtual std::string PopulateConfig(TiXmlElement* node, LevelTemplate& conf);
	virtual Level* CreateInstance(const LevelTemplate& conf);
};

#endif
