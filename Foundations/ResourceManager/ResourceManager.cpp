#include "ResourceManager.h"

#include "IwResManager.h"
#include "MemoryBuckets.h"
#include "Debug.h"

bool ResourceManager::s_bUseSpecificMemoryBucket = false;
uint32 ResourceManager::s_uiMemoryBucketId = IW_MEM_BUCKET_ID_SYSTEM;

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

void ResourceManager::Initialize(uint32 memorybucketid) {
	s_bUseSpecificMemoryBucket = true;
	s_uiMemoryBucketId = s_uiMemoryBucketId = memorybucketid;
	Initialize();
}

void ResourceManager::Terminate() {
	if (s_pxInstance) {
		delete s_pxInstance;
		s_pxInstance = NULL;
	}
	s_bUseSpecificMemoryBucket = false;
}

ResourceManager& ResourceManager::GetInstance() {
	if (!s_pxInstance) {
		s_pxInstance = new ResourceManager();
	}
	return *s_pxInstance;
}

void ResourceManager::LoadPermament(const std::string &groupfile) {
	IwTrace(MYAPP, ("Loading perm resource '%s'...", groupfile.c_str()));
	if (s_bUseSpecificMemoryBucket) {
		IwMemBucketPush(s_uiMemoryBucketId);
		LoadPermamentInt(groupfile);
		IwMemBucketPop();
	} else {
		LoadPermamentInt(groupfile);
	}
}

void ResourceManager::LoadPermamentInt(const std::string &groupfile) {
	if (m_pxPermGroup) {
		IwGetResManager()->DestroyGroup(m_pxPermGroup);
	}
	IwMemBucketPush(eMemoryBucketResources);
	m_pxPermGroup = IwGetResManager()->LoadGroup(groupfile.c_str());
	IwMemBucketPop();
}

void ResourceManager::LoadTemporary(const std::string &groupfile) {
	IwTrace(MYAPP, ("Loading temp resource '%s'...", groupfile.c_str()));
	if (s_bUseSpecificMemoryBucket) {
		IwMemBucketPush(s_uiMemoryBucketId);
		LoadTemporaryInt(groupfile);
		IwMemBucketPop();
	} else {
		LoadTemporaryInt(groupfile);
	}
}

void ResourceManager::LoadTemporaryInt(const std::string &groupfile) {
	if (m_pxTempGroup) {
		IwGetResManager()->DestroyGroup(m_pxTempGroup);
	}
	IwMemBucketPush(eMemoryBucketResources);
	m_pxTempGroup = IwGetResManager()->LoadGroup(groupfile.c_str());
	m_sTempGroup = groupfile;
	IwMemBucketPop();
}

void ResourceManager::UnloadTemporary() {
	IwTrace(MYAPP, ("Unloading temp resource '%s'...", m_sTempGroup.c_str()));
	if (m_pxTempGroup) {
		IwGetResManager()->DestroyGroup(m_pxTempGroup);
		m_pxTempGroup = NULL;
		m_sTempGroup.clear();
	}
}
