#include "FactoryManager.h"
#include "Debug.h"

#include <fstream>
#include <sstream>

#define FACTORY_FILE_FORMAT_NAMESPACE	"http://www.gigglingbits.com"
#define FACTORY_FILE_FORMAT_VERSION		"2"

ShapeFactory* FactoryManager::s_pxShapeFactory = NULL;
TextureFactory* FactoryManager::s_pxTextureFactory = NULL;
EffectFactory* FactoryManager::s_pxEffectFactory = NULL;
FixtureFactory* FactoryManager::s_pxFixtureFactory = NULL;
BodyFactory* FactoryManager::s_pxBodyFactory = NULL;
PreambleFactory* FactoryManager::s_pxPreambleFactory = NULL;
LevelFactory* FactoryManager::s_pxLevelFactory = NULL;

bool FactoryManager::InitializeFromFiles(std::list<std::string> filenames) {
	IW_CALLSTACK_SELF;

	std::list<std::string> xmldatlist;

	std::list<std::string>::iterator it;
	for (it = filenames.begin(); it != filenames.end(); it++) {
		// http://stackoverflow.com/questions/116038/what-is-the-best-way-to-slurp-a-file-into-a-stdstring-in-c
		std::string filename = *it;
		std::ifstream ifstr(filename.c_str(), std::ios::in | std::ios::binary); 
		std::stringstream sstr; 
		sstr << ifstr.rdbuf(); 
		xmldatlist.push_back(sstr.str()); 
	}

	return InitializeFromStrings(xmldatlist);
}

bool FactoryManager::InitializeFromStrings(std::list<std::string> xmldatalist) {
	IW_CALLSTACK_SELF;

	std::list<TiXmlDocument> xmldoclist;

	std::list<std::string>::iterator it;
	for (it = xmldatalist.begin(); it != xmldatalist.end(); it++) {
		std::string xmldata = *it;

		TiXmlDocument doc;
		doc.Parse(xmldata.c_str());
		if (doc.Error()) {
			IwAssertMsg(MYAPP, false, 
				("Could not parse XML data (%s): %s", 
				doc.ErrorDesc(), xmldata.c_str()));
			return false;
		}

		if (!ValidateStructure(doc)) {
			IwAssertMsg(MYAPP, false, 
				("Data format is invalid: %s", xmldata.c_str()));
			return false;
		}

		xmldoclist.push_back(doc); 
	}

	return Initialize(xmldoclist);
}

bool FactoryManager::ValidateStructure(TiXmlDocument& xmldoc) {
	if (TiXmlElement* rootnode = xmldoc.FirstChildElement("root")) {
		char* pc;
		std::string nmspace((pc = (char*)rootnode->Attribute("xmlns")) ? pc : "");
		std::string version((pc = (char*)rootnode->Attribute("fileformatversion")) ? pc : "");
		if (!nmspace.compare(FACTORY_FILE_FORMAT_NAMESPACE) && !version.compare(FACTORY_FILE_FORMAT_VERSION)) {
			return true;
		} else {
			IwAssertMsg(MYAPP, false, 
				("Could not parse XML data. Expected namespace '%s' and file format version '%s'", 
				FACTORY_FILE_FORMAT_NAMESPACE, FACTORY_FILE_FORMAT_VERSION));
		}
	} else {
		IwAssertMsg(MYAPP, false, 
			("Could not parse XML data. Root node '%s' was not found", "root"));
	}
	return false;
}

bool FactoryManager::Initialize(std::list<TiXmlDocument> xmldoclist) {
	IW_CALLSTACK_SELF;

	bool success = true;

	s_pxShapeFactory = new ShapeFactory();
	success &= s_pxShapeFactory->Initialize(xmldoclist);

	s_pxTextureFactory = new TextureFactory();
	success &= s_pxTextureFactory->Initialize(xmldoclist);

	s_pxEffectFactory = new EffectFactory();
	success &= s_pxEffectFactory->Initialize(xmldoclist);

	s_pxFixtureFactory = new FixtureFactory();
	success &= s_pxFixtureFactory->Initialize(xmldoclist);

	s_pxBodyFactory = new BodyFactory();
	success &= s_pxBodyFactory->Initialize(xmldoclist);

	s_pxPreambleFactory = new PreambleFactory();
	success &= s_pxPreambleFactory->Initialize(xmldoclist);

	s_pxLevelFactory = new LevelFactory();
	success &= s_pxLevelFactory->Initialize(xmldoclist);

	return success;
}

void FactoryManager::Terminate() {
	if (s_pxLevelFactory) {
		delete s_pxLevelFactory;
		s_pxLevelFactory = NULL;
	}
	if (s_pxPreambleFactory) {
		delete s_pxPreambleFactory;
		s_pxPreambleFactory = NULL;
	}
	if (s_pxBodyFactory) {
		delete s_pxBodyFactory;
		s_pxBodyFactory = NULL;
	}
	if (s_pxFixtureFactory) {
		delete s_pxFixtureFactory;
		s_pxFixtureFactory = NULL;
	}
	if (s_pxEffectFactory) {
		delete s_pxEffectFactory;
		s_pxEffectFactory = NULL;
	}
	if (s_pxTextureFactory) {
		delete s_pxTextureFactory;
		s_pxTextureFactory = NULL;
	}
	if (s_pxShapeFactory) {
		delete s_pxShapeFactory;
		s_pxShapeFactory = NULL;
	}
}

ShapeFactory& FactoryManager::GetShapeFactory() {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, s_pxShapeFactory, ("The factory is not initialized. Run the Initialize[..]() functions in the manager class first."));
	return *s_pxShapeFactory;
}

TextureFactory& FactoryManager::GetTextureFactory() {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, s_pxTextureFactory, ("The factory is not initialized. Run the Initialize() function in the manager class first."));
	return *s_pxTextureFactory;
}

EffectFactory& FactoryManager::GetEffectFactory() {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, s_pxEffectFactory, ("The factory is not initialized. Run the Initialize() function in the manager class first."));
	return *s_pxEffectFactory;
}

FixtureFactory& FactoryManager::GetFixtureFactory() {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, s_pxFixtureFactory, ("The factory is not initialized. Run the Initialize() function in the manager class first."));
	return *s_pxFixtureFactory;
}

BodyFactory& FactoryManager::GetBodyFactory() {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, s_pxShapeFactory, ("The factory is not initialized. Run the Initialize() function in the manager class first."));
	return *s_pxBodyFactory;
}

PreambleFactory& FactoryManager::GetPreambleFactory() {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, s_pxPreambleFactory, ("The factory is not initialized. Run the Initialize() function in the manager class first."));
	return *s_pxPreambleFactory;
}

LevelFactory& FactoryManager::GetLevelFactory() {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, s_pxShapeFactory, ("The factory is not initialized. Run the Initialize() function in the manager class first."));
	return *s_pxLevelFactory;
}
