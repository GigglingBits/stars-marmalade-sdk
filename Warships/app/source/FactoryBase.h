#ifndef __FACTORYBASE_H__
#define __FACTORYBASE_H__

#include <map>
#include <list>
#include <string>
#include <sstream>

#include "IwDebug.h"
#include "Debug.h"
#include "s3eTimer.h"
#include "tinyxml.h"

template<class TConf, class TProd>
class FactoryBase {
private:
	TConf m_xDefaultConfig;
	std::map<std::string, TConf> m_xConfigs;

protected:
	std::string m_sRootName;
	std::string m_sGroupName;
	std::string m_sElementName;

	// loads all configurations from an XML node, parses them, and adds them to the 
	// configuration cache (map)
	virtual void LoadConfigs(TiXmlElement* element) {
		IW_CALLSTACK_SELF;
		TiXmlElement* node = element->FirstChildElement(m_sElementName.c_str());
		while (node) {
			AddConfig(node);
			node = node->NextSiblingElement();
		}
	}

	// reads config from an XML element, and populates a type safe configuration stucture
	virtual std::string PopulateConfig(TiXmlElement* node, TConf& conf) = 0;

	// creates a product based on a configuration structure
	virtual TProd* CreateInstance(const TConf& conf) = 0;

protected:
	FactoryBase(const std::string& rootname, const std::string& groupname, const std::string& elementname) {
		m_sRootName = rootname;
		m_sGroupName = groupname;
		m_sElementName = elementname;
	}
    
    virtual ~FactoryBase() {}

    bool Initialize(TiXmlDocument xmldoc) {
		IW_CALLSTACK_SELF;
		if (TiXmlElement* rootnode = xmldoc.FirstChildElement(m_sRootName.c_str())) {
			TiXmlElement* node;
			if ((node = rootnode->FirstChildElement(m_sGroupName.c_str()))) {
				LoadConfigs(node);
			}
			return true;
		} else {
			IwAssertMsg(MYAPP, false, 
				("Could not parse XML data. Root node '%s' was not found: %s", m_sRootName.c_str(), xmldoc.ErrorDesc()));
		}
		return false;
	}
    
	std::string GenerateInstanceId() {
		// todo: for improved debugging, the config id
		//       should be used as part if the instance id
		return GenerateUniqueId(0);
	}

	std::string GenerateUniqueId(const std::string& prefix, long seed = 0) {
		std::ostringstream os;
		os << prefix << ":_";

		if (seed == 0) {
			os << seed << "_";
		}

		static uint counter = 0;
		os << counter++;

		return std::string(os.str());
	}

public:
    bool Initialize(std::list<TiXmlDocument> xmldoclist) {
		IW_CALLSTACK_SELF;
		bool success = true;
		std::list<TiXmlDocument>::iterator it;
		for (it = xmldoclist.begin(); it != xmldoclist.end(); it++) {
			TiXmlDocument xmldoc = *it;
			success &= Initialize(xmldoc);
		}
		return success;
	}

	std::string GenerateUniqueId(long seed = 0) {
		return GenerateUniqueId(m_sElementName, seed);
	}

	std::string AddConfig(TiXmlElement* node) {
		IW_CALLSTACK_SELF;
		// todo: assert that node contains m_sElementName

		TConf conf;
		std::string id = PopulateConfig(node, conf);
		if (id.empty()) {
			id = GenerateUniqueId((long)node);
		}

		IwAssertMsg(MYAPP, !ConfigExists(id), ("Configuration with id '%s' already exists. The old one will be overwritten.", id.c_str()));
		SetConfig(id, conf);

		return id;
	}

	void SetDefaultConfig(const TConf& conf) {
		IW_CALLSTACK_SELF;
		m_xDefaultConfig = conf;
	}

	void SetConfig(const std::string& id, const TConf& conf) {
		IW_CALLSTACK_SELF;
		m_xConfigs[id] = conf;
	}

	const TConf& GetConfig(const std::string& id) {
		IW_CALLSTACK_SELF;
		if (ConfigExists(id)) {
			return m_xConfigs[id];
		}
		IwAssertMsg(MYAPP, false, ("Configuration with id '%s' does not exist. Default configuration will be used instead.", id.c_str()));
		return m_xDefaultConfig;
	}

	bool ConfigExists(const std::string& id) {
		return m_xConfigs.find(id) != m_xConfigs.end();
	}

	TProd* Create(const std::string& id) {
		IW_CALLSTACK_SELF;
		return CreateInstance(GetConfig(id));
	}
};

#endif
