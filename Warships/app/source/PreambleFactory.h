#ifndef __PREAMBLEFACTORY_H__
#define __PREAMBLEFACTORY_H__

#include "FactoryBase.h"
#include "PreambleTemplate.h"
#include "Preamble.h"

class PreambleFactory : public FactoryBase<PreambleTemplate, Preamble> {
public:
	PreambleFactory();

public:
	virtual std::string PopulateConfig(TiXmlElement* node, PreambleTemplate& conf);
	virtual Preamble* CreateInstance(const PreambleTemplate& conf);
};

#endif
