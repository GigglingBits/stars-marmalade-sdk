#include "FixtureFactory.h"

FixtureFactory::FixtureFactory() : FactoryBase<b2FixtureDef, void>("root", "fixtures", "fixture") {
}

std::string FixtureFactory::PopulateConfig(TiXmlElement* node, b2FixtureDef& conf) {
	char* pc;
	std::string id((pc = (char*)node->Attribute("id")) ? pc : GenerateUniqueId((long)node));

	double friction, restitution, density;
	int sensor;
	node->Attribute("friction", &friction);
	node->Attribute("restitution", &restitution);
	node->Attribute("density", &density);
	node->Attribute("sensor", &sensor);

	// build the template
	conf.isSensor = sensor != 0;
	conf.friction = (float32)friction;
	conf.restitution = (float32)restitution;
	conf.density = (float32)density;

	return id;
}

void* FixtureFactory::CreateInstance(const b2FixtureDef& conf) {
	IwAssertMsg(NYAPP, false, ("Cannot create instance of fixture. This is not supported."));
	return NULL;
}
