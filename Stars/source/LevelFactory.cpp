#include "LevelFactory.h"

#include <sstream>
#include <string>

#include "FactoryManager.h"
#include "Debug.h"

LevelFactory::LevelFactory() : FactoryBase<LevelTemplate, Level>("root", "levels", "level") {
}

std::string LevelFactory::PopulateConfig(TiXmlElement* node, LevelTemplate& conf) {
	IW_CALLSTACK_SELF;

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

	// add sequences of level elements
	TiXmlElement* spritesequencenode = node->FirstChildElement("spritesequence");
	while (spritesequencenode) {
		TiXmlElement* linenode = spritesequencenode->FirstChildElement("line");
		std::vector<std::string> lines;
		while (linenode) {
			// read data
			std::string map((pc = (char*)linenode->Attribute("map")) ? pc : "");
			lines.push_back(map);
			
			// move next
			linenode = linenode->NextSiblingElement();
		}
		
		// add element
		conf.AddElements(height, lines);

		// move next
		spritesequencenode = spritesequencenode->NextSiblingElement();
	}
	
	return levelname;
}

Level* LevelFactory::CreateInstance(const LevelTemplate& conf) {
	LevelTemplate leveltpl = conf;

	// create level instance
	Level* level = new Level(leveltpl.GetSize(), leveltpl.GetDustRequirement(), leveltpl.GetBackground());

	// populate level
	LevelTemplate::ElementQueue elems(leveltpl.GetElements());
	while (!elems.empty()) {
		level->Add(
			elems.front().Delay,
			elems.front().BodyName,
			elems.front().Position);
		elems.pop();
	}

	return level;
}
