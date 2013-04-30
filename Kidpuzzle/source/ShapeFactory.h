#ifndef __SHAPEFACTORY_H__
#define __SHAPEFACTORY_H__

#include "FactoryBase.h"
#include "ShapeTemplate.h"

class ShapeFactory : public FactoryBase<ShapeTemplate, void> {
private:
	static const int MAX_POINT_COUNT = 64;

public:
	ShapeFactory();

private:
	int ReadVertices(TiXmlElement* pointnode, float xpoints[MAX_POINT_COUNT], float ypoints[MAX_POINT_COUNT]);

public:
	virtual std::string PopulateConfig(TiXmlElement* node, ShapeTemplate& conf);
	virtual void* CreateInstance(const ShapeTemplate& conf);
};

#endif
