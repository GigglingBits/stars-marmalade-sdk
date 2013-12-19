#include "LevelFactory.h"

#include <sstream>
#include <string>

#include "FactoryManager.h"
#include "Debug.h"

LevelFactory::LevelFactory() : FactoryBase<LevelTemplate, Level>("root", "levels", "level") {
	LevelTemplate tpl;
	tpl.SetName("Default");
	tpl.SetDustRequirement(0);
	tpl.SetSize(5.0f, 5.0f);
	SetDefaultConfig(tpl);
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
		std::string icon((pc = (char*)spritesequencenode->Attribute("icon")) ? pc : "");
		std::string text((pc = (char*)spritesequencenode->Attribute("text")) ? pc : "");
		conf.StartSection(icon, text);
		
		int delay;
		spritesequencenode->Attribute("delay", &delay);
		IwAssertMsg(MYAPP, delay > 0, ("Delay in spritesequence is either not defined or smaller or equal to 0."));

		double speed;
		spritesequencenode->Attribute("speed", &speed);
		IwAssertMsg(MYAPP, speed > 0.0f, ("Delay in spritesequence is either not defined or smaller or equal to 0."));
		
		std::vector<std::string> lines;
		TiXmlElement* linenode = spritesequencenode->FirstChildElement("line");
		while (linenode) {
			// read data
			std::string map((pc = (char*)linenode->Attribute("map")) ? pc : "");
			lines.push_back(map);
			
			// move next
			linenode = linenode->NextSiblingElement();
		}
		
		// add element
		conf.AddElements(height, defs, lines, delay, speed);
		
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
		AddElement(*level, elems.front());
		elems.pop();
	}

	return level;
}

void LevelFactory::AddElement(Level& level, const LevelTemplate::LevelTemplate::LevelElement& element) {
	if (!element.SectionIcon.empty() || !element.SectionText.empty()) {
		level.StartSection(element.SectionIcon, element.SectionText);
	}
	if (element.Delay > 0 || !element.BodyName.empty()) {
		level.Add(
			element.Delay,
			element.BodyName,
			element.Position,
			element.Speed);
	}
}
