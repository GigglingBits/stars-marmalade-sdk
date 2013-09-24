#include "PreambleFactory.h"

#include <sstream>
#include <string>

#include "FactoryManager.h"
#include "Debug.h"

PreambleFactory::PreambleFactory() : FactoryBase<PreambleTemplate, Preamble>("root", "preambles", "preamble") {
}

std::string PreambleFactory::PopulateConfig(TiXmlElement* node, PreambleTemplate& conf) {
	IW_CALLSTACK_SELF;

	char* pc;

	std::string id((pc = (char*)node->Attribute("name")) ? pc : "");
	IwAssertMsg(MYAPP, !id.empty(), ("No level name is defined."));

	conf.SetText(node->GetText());
	conf.SetFile((pc = (char*)node->Attribute("file")) ? pc : "");
	conf.SetTextureId((pc = (char*)node->Attribute("texture")) ? pc : "");

	IwAssertMsg(MYAPP, conf.GetTextureId().empty() || conf.GetFile().empty(), ("Both, a file and a texture, are specified in preamble: %s", id.c_str()));

	IwAssertMsg(MYAPP, conf.GetTextureId().empty() || FactoryManager::GetTextureFactory().ConfigExists(conf.GetTextureId()), ("No texture '%s' could be found. It is referenced by preamble '%s'.", conf.GetTextureId().c_str(), id.c_str()));
	
	return id;
}

Preamble* PreambleFactory::CreateInstance(const PreambleTemplate& conf) {
	PreambleTemplate tpl = conf;
	Preamble* p = new Preamble(tpl.GetText(), tpl.GetTextureId(), tpl.GetFile());
	return p;
}
