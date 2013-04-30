#include "LevelFactory.h"

#include <sstream>
#include <string>

#include "FactoryManager.h"

LevelFactory::LevelFactory() : FactoryBase<LevelTemplate, Level>("root", "levels", "level") {
}

std::string LevelFactory::PopulateConfig(TiXmlElement* node, LevelTemplate& conf) {
	char* pc;
	std::string levelname((pc = (char*)node->Attribute("name")) ? pc : "");
	std::string background((pc = (char*)node->Attribute("background")) ? pc : "");

	double width, height;
	node->Attribute("width", &width);
	node->Attribute("height", &height);

	IwAssertMsg(MYAPP, !levelname.empty(), ("No level name is defined."));
	IwAssertMsg(MYAPP, std::abs(width) > 0.1f, ("Level is not wide enough!"));
	IwAssertMsg(MYAPP, std::abs(height) > 0.1f, ("Level is not tall enough!"));

	conf.SetName(levelname);
	conf.SetBackground(background);
	conf.SetSize((float)width, (float)height);

	// add level elements
	TiXmlElement* subnode = node->FirstChildElement("sprite");
	while (subnode) {
		// read data
		std::string elementid((pc = (char*)subnode->Attribute("id")) ? pc : GenerateUniqueId("sprite", (long)subnode));
		std::string bodyname((pc = (char*)subnode->Attribute("body")) ? pc : "");

		if (bodyname.empty()) {
			if (TiXmlElement* bodynode = subnode->FirstChildElement("body")) {
				bodyname = FactoryManager::GetBodyFactory().AddConfig(bodynode);
			}
		}

		IwAssertMsg(MYAPP, FactoryManager::GetBodyFactory().ConfigExists(bodyname), ("Body '%s' could not be found. Its referenced by level '%s'.", bodyname.c_str(), levelname.c_str()));

		double angle, xpos, ypos;
		subnode->Attribute("angle", &angle);
		subnode->Attribute("xpos", &xpos);
		subnode->Attribute("ypos", &ypos);

		// add element
		conf.AddElement(elementid, bodyname, CIwFVec2((float)xpos, (float)ypos), (float)angle);

		// move next
		subnode = subnode->NextSiblingElement();
	}

	conf.Validate();
	return levelname;
}

Level* LevelFactory::CreateInstance(const LevelTemplate& conf) {
	LevelTemplate leveltpl = conf;

	// create level instance
	Level* level = new Level(leveltpl.GetSize(), leveltpl.GetBackground());

	// populate level
	LevelTemplate::ElementMap::iterator iter;
	for (iter = leveltpl.GetElements().begin(); iter != leveltpl.GetElements().end(); ++iter) {
		// create sprite
		LevelTemplate::LevelElement& element = iter->second;
		Body* body = FactoryManager::GetBodyFactory().CreateWithLocation(element.BodyName, element.Position, element.Angle);

		// in the context of the level, the sprites
		// have more specific ids
		std::string id = iter->first;
		if (!id.empty()) {
			body->SetId(id);
		}

		// add to level
		level->Add(body);
	}

	return level;
}
