#include <string>
#include "s3e.h"

#include "Test.h"

#include "Configuration.h"
#include "Page.h"
#include "PageMap.h"
#include "Spectator.h"

void Test::RunTest() {
	IwMemBucketPush(IW_MEM_BUCKET_ID_SYSTEM);
	uint32 systemcheckpoint = IwMemBucketCheckpoint();
	IwMemBucketPush(IW_MEM_BUCKET_ID_SYSTEM_DEBUG);
	uint32 testcheckpoint = IwMemBucketCheckpoint();

	RunStringTest();
	Evaluate(systemcheckpoint, testcheckpoint);
    
	RunJpegFileTest();
	Evaluate(systemcheckpoint, testcheckpoint);
    
	RunPageTest();
	Evaluate(systemcheckpoint, testcheckpoint);

	RunPageMapTest();
	Evaluate(systemcheckpoint, testcheckpoint);

	RunSpectatorTest();
	Evaluate(systemcheckpoint, testcheckpoint);
	
	//RunAssetTest();
	//Evaluate(systemcheckpoint, testcheckpoint);
	//
	IwMemBucketPop();
	IwMemBucketPop();
}

void Test::RunPageMapTest() {
	PageMap* map = new PageMap();
	map->Load(Configuration::GetInstance().PageMapFile);
	delete map;
}

//void Test::RunAssetTest() {
//	PageMap* map = new PageMap();
//	map->SetLanguage("en");
//	map->Load(Configuration::GetInstance().PageMapFile);
//
//	int groupcount = map->GetGroupCount();
//	int imagecount = map->GetImageCount();
//	
//	for (int i = 0; i < groupcount; i++) {
//		for (int k = 0; k < imagecount; k++) {
//			Page& page = map->GetImage(i, k);			
//			if (!page.m_sSoundName.empty()) {
//				IwAssertMsg(MYAPP, CheckFileExists(page.m_sSoundName), ("Sound file for page '%s' not found: '%s'", page.m_sPageName.c_str(), page.m_sSoundName.c_str()));
//			}
//			if (!page.m_sSpeechName.empty()) {
//				IwAssertMsg(MYAPP, CheckFileExists(page.m_sSpeechName), ("Speech file for page '%s' not found: '%s'", page.m_sPageName.c_str(), page.m_sSpeechName.c_str()));
//			}
//			if (!page.m_sImageName.empty()) {
//				IwAssertMsg(MYAPP, CheckFileExists(page.m_sImageName), ("Image file for page '%s' not found: '%s'", page.m_sPageName.c_str(), page.m_sImageName.c_str()));
//			}
//		}
//	}
//
//	delete map;
//}

void Test::RunSpectatorTest() {
	PageMap map(Configuration::GetInstance().PageMapFile, "en", "en_GB");
	Spectator* spectator = new Spectator(map);

	spectator->Move(eUp);
	spectator->Move(eRight);
	spectator->Move(eDown);
	spectator->Move(eLeft);

	delete spectator;
}

void Test::RunStringTest() {
    std::string s("abc");
    IwAssert(MYAPP, s == "abc");
    
    s += "def";
    IwAssert(MYAPP, s == "abcdef");
    
    s = s + "ghi";
    IwAssert(MYAPP, s == "abcdefghi");
}

void Test::RunPageTest() {
	Page* page = new Page();
	page->m_sPageId = "1 / 1";
	page->m_sPageName = "Test dummy";
	page->m_sSoundName = "sounds/rooster.mp3";
    
	//page->SetThumbnail("thumbnails/lemur.jpg");
	page->SetImage("images/lemur.jpg");
    
	page->SetActive(true);
	page->SetActive(true);
	page->SetActive(false);
	page->SetActive(false);
    
	delete page;
}

void Test::RunJpegFileTest() {
	JpegFile* pFile = new JpegFile();
	pFile->LoadFile("images/lemur.jpg");
	JpegDecompressor* pDecomp = pFile->CreateDecompressor();
	while (pDecomp->Step()) {}
	CIwTexture* pTexture = pDecomp->GetTexture();
	delete pTexture;
	delete pDecomp;
	delete pFile;
}

void Test::Evaluate(uint32 systemcheckpoint, uint32 testcheckpoint) {
	uint32 allocid;
	allocid = IwMemBucketDebugCheck(IW_MEM_BUCKET_ID_SYSTEM_DEBUG, testcheckpoint, DBG_FILE);
	if (allocid != 0) {
		IwAssertMsg(MYAPP, false, ("Test failed; memory leaked in debug bucket! Allocation ID: %i", allocid));
		IwMemBucketVisualDump("memdump_sys_post.html", IW_MEM_BUCKET_ID_SYSTEM_DEBUG);
	}
	allocid = IwMemBucketDebugCheck(IW_MEM_BUCKET_ID_SYSTEM, systemcheckpoint, DBG_FILE);
	if (allocid != 0) {
		IwAssertMsg(MYAPP, false, ("Test failed; memory leaked in system bucket! Allocation ID: %i", allocid));
		IwMemBucketVisualDump("memdump_dbg_post.html", IW_MEM_BUCKET_ID_SYSTEM);
	}
}

bool Test::CheckFileExists(std::string filename) {
	s3eFile* file;
	if ((file = s3eFileOpen(filename.c_str(), "r"))) {
		s3eFileClose(file);
		return true;
	}
	return false;
}
