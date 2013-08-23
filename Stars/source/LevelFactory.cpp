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

	double width, height, dustrequirement;
	node->Attribute("width", &width);
	node->Attribute("height", &height);
	node->Attribute("dustrequirement", &dustrequirement);

	IwAssertMsg(MYAPP, !levelname.empty(), ("No level name is defined."));
	IwAssertMsg(MYAPP, std::abs(width) > 0.1f, ("Level is not wide enough!"));
	IwAssertMsg(MYAPP, std::abs(height) > 0.1f, ("Level is not tall enough!"));
	IwAssertMsg(MYAPP, dustrequirement > 0.0f, ("Level does not have any dust requirements! Should be more than 0.0."));

	conf.SetName(levelname);
	conf.SetBackground(background);
	conf.SetSize((float)width, (float)height);
	conf.SetDustRequirement((float)dustrequirement);

	// add level elements
	TiXmlElement* subnode = node->FirstChildElement("sprite");
	while (subnode) {
		// read data
		std::string bodyname((pc = (char*)subnode->Attribute("body")) ? pc : "");

		if (bodyname.empty()) {
			if (TiXmlElement* bodynode = subnode->FirstChildElement("body")) {
				bodyname = FactoryManager::GetBodyFactory().AddConfig(bodynode);
			}
		}

		IwAssertMsg(MYAPP, FactoryManager::GetBodyFactory().ConfigExists(bodyname), ("Body '%s' could not be found. Its referenced by level '%s'.", bodyname.c_str(), levelname.c_str()));

		double pos = 0.0f;
		subnode->Attribute("pos", &pos);

		int delay = -1;
		subnode->Attribute("delay", &delay);

		// add element
		conf.AddElement(bodyname, (float)pos, (uint16)(delay >= 0 ? delay : 0));

		// move next
		subnode = subnode->NextSiblingElement();
	}

	return levelname;
}

Level* LevelFactory::CreateInstance(const LevelTemplate& conf) {
	LevelTemplate leveltpl = conf;

	// create level instance
	Level* level = new Level(leveltpl.GetSize(), leveltpl.GetDustRequirement(), leveltpl.GetBackground());

	// populate level
	GameFoundation& game = level->GetGameFoundation();
	LevelTemplate::ElementQueue elems(leveltpl.GetElements());
	while (!elems.empty()) {
		game.Add(
			elems.front().Delay,
			elems.front().BodyName,
			elems.front().Position);
		elems.pop();
	}

	return level;
}
