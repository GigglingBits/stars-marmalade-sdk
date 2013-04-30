#ifndef __PREAMBLETEMPLATE_H__
#define __PREAMBLETEMPLATE_H__

#include <string>

class PreambleTemplate {
private:
	std::string m_sText;
	
public:
	void Set(std::string text);
	std::string GetText();
};

#endif
