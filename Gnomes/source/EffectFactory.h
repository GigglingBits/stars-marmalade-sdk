#ifndef __EFFECTFACTORY_H__
#define __EFFECTFACTORY_H__

#include "FactoryBase.h"
#include "EffectTemplate.h"
#include "LocalEffect.h"

class EffectFactory : public FactoryBase<EffectTemplate, LocalEffect> {
public:
	EffectFactory();

public:
	virtual std::string PopulateConfig(TiXmlElement* node, EffectTemplate& conf);
	virtual LocalEffect* CreateInstance(const EffectTemplate& conf);
};

#endif
