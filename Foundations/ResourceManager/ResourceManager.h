#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

#include <string>
#include <map>

#include "IwResGroup.h"

class ResourceManager {
private:
	static ResourceManager* s_pxInstance;
	ResourceManager();
	~ResourceManager();
	
public:
	static void Initialize();
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
};

#endif
