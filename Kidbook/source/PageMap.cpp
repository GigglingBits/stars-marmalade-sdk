#include <string>

#include "PageMap.h"
#include "Configuration.h"
#include "Page.h"

PageMap::PageMap() {
	m_iRowCount = 0;
	m_iColCount = 0;
}

PageMap::PageMap(const std::string& filename, const std::string& lang, const std::string& locale) {
	SetLanguage(lang);
	Load(filename);
}

PageMap::~PageMap() {
	ClearMap();
}  

void PageMap::SetLanguage(std::string lang) {
	m_sLanguage = lang;
}

void PageMap::SetLocale(std::string locale) {
	m_sLocale = locale;
}

int PageMap::GetColCount() {
	return m_iColCount;
}

int PageMap::GetRowCount() {
	return m_iRowCount;
}

int PageMap::ToArrayIndex(int x, int y) {
	// normalize coords
	int xskip = y / m_iRowCount;
	int yskip = x / m_iColCount;

	x = (x + xskip) % m_iColCount;
	y = (y + yskip) % m_iRowCount;

	x = x < 0 ? m_iColCount + x : x;
	y = y < 0 ? m_iRowCount + y : y;

	// calculate index
	return y * m_iColCount + x;
}

Page& PageMap::GetImage(int x, int y) {
	return m_xMap[ToArrayIndex(x, y)];
}

void PageMap::Load(std::string filename) {
	TiXmlDocument doc(filename.c_str());
	if (doc.LoadFile()) {
		if (TiXmlElement* rootnode = doc.FirstChildElement("root")) {
			if (TiXmlElement* mapnode = rootnode->FirstChildElement("map")) {
				LoadMap(*mapnode);
			}
		}
	}
}

void PageMap::LoadMap(const TiXmlElement& mapnode) {
	// prepare the dimensions
	const char* pcText;
	if ((pcText = mapnode.Attribute("rows"))) {
		m_iRowCount = atoi(pcText);
	}
	if ((pcText = mapnode.Attribute("cols"))) {
		m_iColCount = atoi(pcText);
	}
	m_xMap.resize(m_iRowCount * m_iColCount);

	// fill map
	const TiXmlElement* pagenode = mapnode.FirstChildElement("page");
	for (uint32 cursor = 0; pagenode && cursor < m_xMap.size(); cursor++) {
		Page& page = m_xMap[cursor];
		LoadPage(page, *pagenode);
		pagenode = pagenode->NextSiblingElement("page");
	}	
}

void PageMap::LoadPage(Page& page, const TiXmlElement& pagenode) {
	std::string imagename;
	const char* pcText;

	// read data from XML
	pcText = pagenode.Attribute("id");
	page.m_sPageId = pcText ? pcText : "";
	IwAssertMsg(MYAPP, !page.m_sPageId.empty(), ("Page has no ID. All pages must have one!"));

	const TiXmlElement* pagesettingnode = pagenode.FirstChildElement();
	while (pagesettingnode) {
		if (!strcmp(pagesettingnode->Value(), "image")) {
			pcText = pagesettingnode->Attribute("file");
			imagename = pcText ? pcText : "";
		} else if (!strcmp(pagesettingnode->Value(), "focuspoint")) {
			if ((pcText = pagesettingnode->Attribute("x"))) {
				float xpos = (float)atof(pcText);
				if ((pcText = pagesettingnode->Attribute("y"))) {
					float ypos = (float)atof(pcText);
					page.SetFocusPoint(CIwFVec2(xpos, ypos));
				}
			}
		} else if (!strcmp(pagesettingnode->Value(), "sound")) {
			pcText = pagesettingnode->Attribute("file");
			page.m_sSoundName = pcText ? pcText : "";
		} else if (!strcmp(pagesettingnode->Value(), "speech")) {
			//if (!m_sLanguage.empty()) {
			//	pcText = pagesettingnode->Attribute("file");
			//	page.m_sSpeechName = pcText ? pcText : "";
			//}
			// use m_sLocale for checking if Switzerland...
		} else if (!strcmp(pagesettingnode->Value(), "text")) {
			if (!strcmp(pagesettingnode->Attribute("lang"), m_sLanguage.c_str())) {
				pcText = pagesettingnode->GetText();
				page.m_sPageName = pcText ? pcText : "";
			}
		}
		pagesettingnode = pagesettingnode->NextSiblingElement();
	}

	// verify and validate data
	std::string filename = GetVerifiedFileName("thumbnails", page.m_sPageId, imagename, ".jpg");
	IwAssertMsg(MYAPP, !filename.empty(), ("Page '%s' has no thumbnail!", page.m_sPageId.c_str()));
	page.SetThumbnail(filename);
	
	filename = GetVerifiedFileName("images", page.m_sPageId, imagename, ".jpg");
	IwAssertMsg(MYAPP, !filename.empty(), ("Page '%s' has no image!", page.m_sPageId.c_str()));
	page.SetImage(filename);

	page.m_sSoundName = GetVerifiedFileName("sounds", page.m_sPageId, page.m_sSoundName, ".mp3");
	if (page.m_sSoundName.empty()) {
		IwTrace(MYAPP, ("failed to locate sound for page '%s'!", page.m_sPageId.c_str()));
	}

	page.m_sSpeechName = "";
	if (Configuration::GetInstance().EnableSpeech && !m_sLanguage.empty()) {
		page.m_sSpeechName = GetVerifiedFileName( std::string("speech/") + m_sLanguage, page.m_sPageId, page.m_sSpeechName, ".mp3");
		if (page.m_sSpeechName.empty()) {
			IwTrace(MYAPP, ("failed to locate speech for page '%s'!", page.m_sPageId.c_str()));
		}
	}
}

std::string PageMap::GetVerifiedFileName(const std::string& path, const std::string& pageid, const std::string& filename, const std::string& suffix) {
	std::string fullname;
	if (!filename.empty()) {
		fullname = std::string(path).append("/").append(filename);
		if (s3eFileCheckExists(fullname.c_str())) {
			return fullname;
		}
	}
	fullname = std::string(path).append("/").append(pageid).append(suffix);
	if (s3eFileCheckExists(fullname.c_str())) {
		return fullname;
	}
	return "";
}

void PageMap::ClearMap() {
	for (uint32 cursor = 0; cursor < m_xMap.size(); cursor++) {
		m_xMap[cursor].SetActive(false);
	}
	m_xMap.clear_optimised();
}
