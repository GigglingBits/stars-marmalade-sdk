#include "ResourceManager.h"

#include "IwResManager.h"
#include "Debug.h"

ResourceManager* ResourceManager::s_pxInstance = NULL;

ResourceManager::ResourceManager() : m_pxPermGroup(NULL), m_pxTempGroup(NULL) {
}

ResourceManager::~ResourceManager() {
	if (m_pxTempGroup) {
		IwGetResManager()->DestroyGroup(m_pxTempGroup);
	}
	if (m_pxPermGroup) {
		IwGetResManager()->DestroyGroup(m_pxPermGroup);
	}
}

void ResourceManager::Initialize() {
	GetInstance();
}

void ResourceManager::Terminate() {
	if (s_pxInstance) {
		delete s_pxInstance;
		s_pxInstance = NULL;
	}
}

ResourceManager& ResourceManager::GetInstance() {
	if (!s_pxInstance) {
		s_pxInstance = new ResourceManager();
	}
	return *s_pxInstance;
}

void ResourceManager::LoadPermament(const std::string &groupfile) {
	IwTrace(MYAPP, ("Loading perm resource '%s'...", groupfile.c_str()));
	
	if (m_pxPermGroup) {
		IwGetResManager()->DestroyGroup(m_pxPermGroup);
	}
	m_pxPermGroup = IwGetResManager()->LoadGroup(groupfile.c_str());
}

void ResourceManager::LoadTemporary(const std::string &groupfile) {
	IwTrace(MYAPP, ("Loading temp resource '%s'...", groupfile.c_str()));
	
	if (m_pxTempGroup) {
		IwGetResManager()->DestroyGroup(m_pxTempGroup);
	}
	m_pxTempGroup = IwGetResManager()->LoadGroup(groupfile.c_str());
	m_sTempGroup = groupfile;
}

void ResourceManager::UnloadTemporary() {
	IwTrace(MYAPP, ("Unloading temp resource '%s'...", m_sTempGroup.c_str()));
	if (m_pxTempGroup) {
		IwGetResManager()->DestroyGroup(m_pxTempGroup);
		m_pxTempGroup = NULL;
		m_sTempGroup.clear();
	}
}
