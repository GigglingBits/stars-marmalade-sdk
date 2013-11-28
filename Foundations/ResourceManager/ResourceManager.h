#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

#include <string>
#include <map>

#include "IwResGroup.h"

class ResourceManager {
private:
	static bool s_bUseSpecificMemoryBucket;
	static uint32 s_uiMemoryBucketId;
	
	static ResourceManager* s_pxInstance;

	ResourceManager();
	~ResourceManager();
	
public:
	static void Initialize();
	static void Initialize(uint32 memorybucketid);
	static void Terminate();

	static ResourceManager& GetInstance();
	
private:
	CIwResGroup* m_pxPermGroup;
	
	CIwResGroup* m_pxTempGroup;
	std::string m_sTempGroup;
	
public:
	void LoadPermament(const std::string& groupfile);

	void LoadTemporary(const std::string& groupfile);
	void UnloadTemporary();
	
private:
	void LoadPermamentInt(const std::string& groupfile);
	void LoadTemporaryInt(const std::string& groupfile);
};

#endif
