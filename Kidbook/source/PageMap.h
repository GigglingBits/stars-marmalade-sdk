//--------------------------------------------------------------------------
// Contains the universe of pages
//--------------------------------------------------------------------------

#ifndef _PAGEMAP_H_ 
#define _PAGEMAP_H_ 

#include <string>
#include "tinyxml.h"

#include "Page.h"

class PageMap {
private:
	CIwArray<Page> m_xMap;
	int m_iColCount;
	int m_iRowCount;

	std::string m_sLanguage;
	std::string m_sLocale;

public:
	PageMap();
	PageMap(const std::string& filename, const std::string& lang, const std::string& locale);
	~PageMap();

	void SetLanguage(std::string lang);
	void SetLocale(std::string locale);
	void Load(std::string filename);

	int GetColCount();
	int GetRowCount();
	
	Page& GetImage(int x, int y);

private:
	void LoadMap(const TiXmlElement& mapnode);
	void LoadPage(Page& page, const TiXmlElement& pagenode);
	std::string GetVerifiedFileName(const std::string& path, const std::string& pageid, const std::string& filename, const std::string& suffix);

	int ToArrayIndex(int x, int y);
	void ClearMap();
};

#endif
