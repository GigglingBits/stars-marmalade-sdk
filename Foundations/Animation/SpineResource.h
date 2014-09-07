#ifndef __SPINERESOURCE__
#define __SPINERESOURCE__

#include <string>
#include "IwResource.h"

#define SPINERESOURCE_TYPE "SpineResource"

class SpineResource : public CIwResource {
private:
	std::string m_sJsonFile;
	std::string m_sJsonData;

	std::string m_sAtlasFile;
	std::string m_sAtlasData;
	
public:
	IW_MANAGED_DECLARE(SpineResource);
		
	inline const std::string& GetJsonData() const	{ return m_sJsonData; };
	inline const std::string& GetAtlasData() const	{ return m_sAtlasData; };

public:
	virtual void Serialise();
    virtual bool ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
    virtual void ParseClose(CIwTextParserITX* pParser);
	
private:
	std::string GetFileData(const std::string& filename);

	void SerialiseString(std::string& s);
};

#endif
