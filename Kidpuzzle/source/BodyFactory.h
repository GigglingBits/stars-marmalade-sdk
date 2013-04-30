#ifndef __BODYFACTORY_H__
#define __BODYFACTORY_H__

#include "FactoryBase.h"
#include "BodyTemplate.h"
#include "Body.h"

class BodyFactory : public FactoryBase<BodyTemplate, Body> {
public:
	BodyFactory();

public:
	virtual std::string PopulateConfig(TiXmlElement* node, BodyTemplate& conf);
	virtual Body* CreateInstance(const BodyTemplate& conf);
	Body* CreateWithLocation(const std::string& id, const CIwFVec2& position, float32 angle);
};

#endif
