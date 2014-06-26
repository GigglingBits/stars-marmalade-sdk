#include "ResourceManager.h"

#include "IwResManager.h"
#include "IwGxFont.h"
#include "MemoryBuckets.h"
#include "s3eFile.h"
#include "Debug.h"

#include <fstream>

bool ResourceManager::s_bUseSpecificMemoryBucket = false;
uint32 ResourceManager::s_uiMemoryBucketId = IW_MEM_BUCKET_ID_SYSTEM;

ResourceManager* ResourceManager::s_pxInstance = NULL;

ResourceManager::ResourceManager() : m_pxFontGroup(NULL), m_pxPermGroup(NULL), m_pxTempGroup(NULL) {
	if (IwGetResManager()->GetGroupNamed(RESOURCEMANAGER_FONT_GROUP_NAME, IW_RES_PERMIT_NULL_F)) {
		IwAssertMsg(MYAPP, false, ("Resource group with name %s already exists. No new group will be created.", RESOURCEMANAGER_FONT_GROUP_NAME));
	} else {
		IwMemBucketPush(s_uiMemoryBucketId);
		m_pxFontGroup = new CIwResGroup(RESOURCEMANAGER_FONT_GROUP_NAME);
		m_pxFontGroup->SetShared(true);
		IwGetResManager()->AddGroup(m_pxFontGroup);
		IwMemBucketPop();
	}
}

ResourceManager::~ResourceManager() {
	if (m_pxTempGroup) {
		IwGetResManager()->DestroyGroup(m_pxTempGroup);
	}
	if (m_pxPermGroup) {
		IwGetResManager()->DestroyGroup(m_pxPermGroup);
	}
	if (m_pxFontGroup) {
		IwGetResManager()->DestroyGroup(m_pxFontGroup);
	}
}

void ResourceManager::Initialize() {
	GetInstance();
}

void ResourceManager::Initialize(uint32 memorybucketid) {
	s_bUseSpecificMemoryBucket = true;
	s_uiMemoryBucketId = memorybucketid;
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

void ResourceManager::LoadFontFileToResource(const std::string& ttffilename, const std::string& resname, int pointsize) {
	std::vector<char>& data = GetFontData(ttffilename);
	if (data.size() < 1) {
		IwAssertMsg(MYAPP, false, ("Could not retrieve data from TTF file: %s. Font will not be loaded.", ttffilename.c_str()));
		return;
	}
	
	if (m_pxFontGroup) {
		IwMemBucketPush(eMemoryBucketResources);
		if (CIwGxFont* font = IwGxFontCreateTTFontFromBuffer((uint8*)&data[0], data.size(), pointsize)) {
			font->SetName(resname.c_str());
			m_pxFontGroup->AddRes(IW_GX_RESTYPE_FONT, font);
		} else {
			IwAssertMsg(MYAPP, false, ("Could not create font from TTF file: %s. Font will not be loaded.", ttffilename.c_str()));
		}
		IwMemBucketPop();
	} else {
		IwAssertMsg(MYAPP, false, ("Could not create resource group for the fonts. Font will not be loaded."));
	}
}

std::vector<char>& ResourceManager::GetFontData(const std::string& ttffilename) {
	if (m_xFontDataCache.find(ttffilename) == m_xFontDataCache.end()) {
		std::ifstream ifs(ttffilename.c_str(), std::ios::binary);
		if (ifs.good()) {
			IwMemBucketPush(eMemoryBucketResources);
			std::vector<char>& cache = m_xFontDataCache[ttffilename];
			cache.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
			IwMemBucketPop();
		} else {
			IwAssertMsg(MYAPP, false, ("Could not open file: %s. Data will not be loaded.", ttffilename.c_str()));
		}
	}
	return m_xFontDataCache[ttffilename];
}
