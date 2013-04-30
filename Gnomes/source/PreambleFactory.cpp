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
	if (TiXmlElement* preamblenode = node->FirstChildElement("preamble")) {
		text = preamblenode->GetText();
	}

	conf.Set(text);
	return id;
}

Preamble* PreambleFactory::CreateInstance(const PreambleTemplate& conf) {
	PreambleTemplate tpl = conf;
	Preamble* p = new Preamble(tpl.GetText());
	return p;
}
