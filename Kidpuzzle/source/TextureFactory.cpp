#include "TextureFactory.h"

TextureFactory::TextureFactory() : FactoryBase<TextureTemplate, Texture>("root", "textures", "texture") {
	TextureTemplate tpl;
	tpl.AddFrame("_default", 100, "image_missing", "", 0, 0, "");
	SetDefaultConfig(tpl);
}

std::string TextureFactory::PopulateConfig(TiXmlElement* node, TextureTemplate& conf) {
	char* pc;
	std::string id((pc = (char*)node->Attribute("id")) ? pc : GenerateUniqueId((long)node));
	int healthlevel = 0;
	node->Attribute("healthlevel", &healthlevel);

	std::string image((pc = (char*)node->Attribute("image")) ? pc : "");
	std::string pattern((pc = (char*)node->Attribute("pattern")) ? pc : "");
	std::string colour((pc = (char*)node->Attribute("colour")) ? pc : "");

	int duration = 0;
	node->Attribute("duration", &duration);
	std::string nextid((pc = (char*)node->Attribute("next")) ? pc : "");

	// build the template
	if (!image.empty() || !pattern.empty() || !colour.empty()) {
		conf.AddFrame(id, healthlevel, image, pattern, HexToColour(colour), duration, nextid);
	}

	TiXmlElement* subnode = node->FirstChildElement("frame");
	while (subnode) {
		std::string frameid((pc = (char*)subnode->Attribute("id")) ? pc : GenerateUniqueId((long)subnode));
		image = (pc = (char*)subnode->Attribute("image")) ? pc : "";

		healthlevel = 0;
		subnode->Attribute("healthlevel", &healthlevel);

		pattern = (pc = (char*)subnode->Attribute("pattern")) ? pc : "";
		colour = (pc = (char*)subnode->Attribute("colour")) ? pc : "";

		duration = 0;
		subnode->Attribute("duration", &duration);
		nextid = (pc = (char*)subnode->Attribute("next")) ? pc : "";

		conf.AddFrame(frameid, healthlevel, image, pattern, HexToColour(colour), duration, nextid);

		subnode = subnode->NextSiblingElement();
	}

	return id;
}

Texture* TextureFactory::CreateInstance(const TextureTemplate& conf) {
	TextureTemplate myconf = conf;
	return new Texture(myconf);
}

uint32 TextureFactory::HexToColour(const std::string& col) {
	if (col.empty()) {
		return 0x00000000;
	} else {
		// inspired by: http://stackoverflow.com/questions/1070497/c-convert-hex-string-to-signed-integer
		uint32 x;    
		std::stringstream ss; 
		ss << std::hex << col; 
		ss >> x; 
		return x;
	}
}
