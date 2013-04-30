#ifndef __PREAMBLETEMPLATE_H__
#define __PREAMBLETEMPLATE_H__

#include <string>

class PreambleTemplate {
private:
	std::string m_sText;
	std::string m_sFile;
	
public:
	void SetText(const std::string& text);
	const std::string& GetText();

	void SetFile(const std::string& filename);
	const std::string& GetFile();
};

#endif
