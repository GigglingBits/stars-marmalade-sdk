#include "ShapeFactory.h"

#include "Debug.h"

ShapeFactory::ShapeFactory() : FactoryBase<ShapeTemplate, void>("root", "shapes", "shape") {
}

int ShapeFactory::ReadVertices(TiXmlElement* pointnode, float xpoints[ShapeFactory::MAX_POINT_COUNT], float ypoints[ShapeFactory::MAX_POINT_COUNT]) {
	int count = 0;
	for (; pointnode && count < MAX_POINT_COUNT; count++) {
		double x, y;
		pointnode->Attribute("x", &x);
		pointnode->Attribute("y", &y);

		xpoints[count] = (float)x;
		ypoints[count] = (float)y;

		pointnode = pointnode->NextSiblingElement();
	}
	return count;
}

std::string ShapeFactory::PopulateConfig(TiXmlElement* node, ShapeTemplate& conf) {
	IW_CALLSTACK_SELF;

	char* pc;
	std::string id((pc = (char*)node->Attribute("id")) ? pc : GenerateUniqueId((long)node));
	std::string type((pc = (char*)node->Attribute("type")) ? pc : "");

	double height, width;
	node->Attribute("height", &height);
	node->Attribute("width", &width);

	// build the template
	float xpoints[MAX_POINT_COUNT];
	float ypoints[MAX_POINT_COUNT];
	if (0 == type.compare("rectangle")) {
		conf.SetRectangle(width, height);
	} else if (0 == type.compare("circle")) {
		conf.SetCircle(width);
	} else if (0 == type.compare("polygon")) {
		int count = ReadVertices(node->FirstChildElement("point"), xpoints, ypoints);
		conf.SetPolygon(count, xpoints, ypoints);
	} else if (0 == type.compare("chain")) {
		int count = ReadVertices(node->FirstChildElement("point"), xpoints, ypoints);
		conf.SetChain(count, xpoints, ypoints);
	} else {
		IwAssertMsg(MYAPP, false, ("Unknown shape type: %s", type.c_str()));
	}
	return id;
}

void* ShapeFactory::CreateInstance(const ShapeTemplate& conf) {
	IwAssertMsg(NYAPP, false, ("Cannot create instance of shape. This is not supported."));
	return NULL;
}
