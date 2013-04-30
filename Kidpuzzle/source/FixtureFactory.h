#ifndef __FIXTUREFACTORY_H__
#define __FIXTUREFACTORY_H__

#include "FactoryBase.h"
#include "Box2D.h"

class FixtureFactory : public FactoryBase<b2FixtureDef, void> {
public:
	FixtureFactory();

public:
	virtual std::string PopulateConfig(TiXmlElement* node, b2FixtureDef& conf);
	virtual void* CreateInstance(const b2FixtureDef& conf);
};

#endif
