#include <sstream>
#include <string>

#include "EffectFactory.h"
#include "FactoryManager.h"
#include "LocalEffect.h"
#include "SplashText.h"
#include "ParticleEffect.h"
#include "Debug.h"

EffectFactory::EffectFactory() : FactoryBase<EffectTemplate, LocalEffect>("root", "effects", "effect") {
}

std::string EffectFactory::PopulateConfig(TiXmlElement* node, EffectTemplate& conf) {
	IW_CALLSTACK_SELF;

	// read from XML
	char* pc;
	std::string id((pc = (char*)node->Attribute("id")) ? pc : GenerateUniqueId((long)node));
	std::string type((pc = (char*)node->Attribute("type")) ? pc : "");
	std::string shape((pc = (char*)node->Attribute("shape")) ? pc : "");
	std::string texture((pc = (char*)node->Attribute("texture")) ? pc : "");

	int duration = 0x7fff;
	node->Attribute("duration", &duration);

	// add new definitions to other factories
	if (shape.empty()) {
		if (TiXmlElement* subnode = node->FirstChildElement("shape")) {
			shape = FactoryManager::GetShapeFactory().AddConfig(subnode);
		}
	}

	if (texture.empty()) {
		if (TiXmlElement* subnode = node->FirstChildElement("texture")) {
			texture = FactoryManager::GetTextureFactory().AddConfig(subnode);
		}
	}

	// populate the configuration
	conf.Set(type, shape, texture, duration);

	// check integity
	IwAssertMsg(MYAPP, FactoryManager::GetShapeFactory().ConfigExists(conf.GetShapeId()), ("No shape '%s' could be found. It is referenced by body '%s'.", shape.c_str(), id.c_str()));
	IwAssertMsg(MYAPP, FactoryManager::GetTextureFactory().ConfigExists(conf.GetTextureId()), ("No texture '%s' could be found. It is referenced by body '%s'.", texture.c_str(), id.c_str()));

	return id;
}

LocalEffect* EffectFactory::CreateInstance(const EffectTemplate& conf) {
	EffectTemplate copyconf(conf);

	TextureTemplate textureconf = FactoryManager::GetTextureFactory().GetConfig(copyconf.GetTextureId());
	ShapeTemplate shapeconf = FactoryManager::GetShapeFactory().GetConfig(copyconf.GetShapeId());
	
	LocalEffect* effect;
	std::string instanceid = GenerateInstanceId();
	if (!copyconf.GetType().compare("splashtext")) {
		effect = new SplashText(instanceid, shapeconf, textureconf);
	} else if (!copyconf.GetType().compare("particles")) {
		effect = new ParticleEffect(instanceid, shapeconf, textureconf);
	} else {
		effect = new LocalEffect(instanceid, shapeconf, textureconf);
	}

	effect->SetFadeTime(copyconf.GetDuration());

	return effect;
}
