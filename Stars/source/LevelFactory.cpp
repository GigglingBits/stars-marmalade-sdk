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

	double width, height, dustrequirement;
	node->Attribute("width", &width);
	node->Attribute("height", &height);
	node->Attribute("dustrequirement", &dustrequirement);

	IwAssertMsg(MYAPP, !levelname.empty(), ("No level name is defined."));
	IwAssertMsg(MYAPP, std::abs(width) > 0.1f, ("Level is not wide enough!"));
	IwAssertMsg(MYAPP, std::abs(height) > 0.1f, ("Level is not tall enough!"));
	IwAssertMsg(MYAPP, dustrequirement > 0.0f, ("Level does not have any dust requirements! Should be more than 0.0."));

	conf.SetName(levelname);
	conf.SetSize((float)width, (float)height);
	conf.SetDustRequirement((float)dustrequirement);
	
	// build sprite defs
	TiXmlElement* spritedefsnode = node->FirstChildElement("spritedefs");
	std::map<char, std::string> defs;
	while (spritedefsnode) {
		TiXmlElement* defnode = spritedefsnode->FirstChildElement("def");
		while (defnode) {
			// read data
			std::string id((pc = (char*)defnode->Attribute("id")) ? pc : "");
			std::string body((pc = (char*)defnode->Attribute("body")) ? pc : "");

			IwAssertMsg(MYAPP, id.length() == 1, ("Invalid spritedef. Ids must consist of one character only."));
			IwAssertMsg(MYAPP, id != " ", ("Invalid spritedef. Ids must not be whitespaces."));
			IwAssertMsg(MYAPP, FactoryManager::GetBodyFactory().ConfigExists(body), ("No body '%s' could be found. It is referenced by level '%s'.", body.c_str(), levelname.c_str()));

			defs[id.at(0)] = body;
			
			// move next
			defnode = defnode->NextSiblingElement();
		}
		// move next
		spritedefsnode = spritedefsnode->NextSiblingElement();
	}
	
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
		conf.AddElements(height, defs, lines);
		
		// move next
		spritesequencenode = spritesequencenode->NextSiblingElement();
	}
	
	return levelname;
}

Level* LevelFactory::CreateInstance(const LevelTemplate& conf) {
	LevelTemplate leveltpl = conf;

	// create level instance
	Level* level = new Level(leveltpl.GetSize(), leveltpl.GetDustRequirement());

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
