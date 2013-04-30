#include "PreambleFactory.h"
#include <sstream>
#include <string>

PreambleFactory::PreambleFactory() : FactoryBase<PreambleTemplate, Preamble>("root", "levels", "level") {
}

std::string PreambleFactory::PopulateConfig(TiXmlElement* node, PreambleTemplate& conf) {
	char* pc;
	std::string id((pc = (char*)node->Attribute("name")) ? pc : "");
	IwAssertMsg(MYAPP, !id.empty(), ("No level name is defined."));

	std::string text;
	std::string file;
	if (TiXmlElement* preamblenode = node->FirstChildElement("preamble")) {
		text = preamblenode->GetText();
		file = (pc = (char*)preamblenode->Attribute("file")) ? pc : "";
	}

	conf.SetText(text);
	conf.SetFile(file);
	return id;
}

Preamble* PreambleFactory::CreateInstance(const PreambleTemplate& conf) {
	PreambleTemplate tpl = conf;
	Preamble* p = new Preamble(tpl.GetText(), tpl.GetFile());
	return p;
}
