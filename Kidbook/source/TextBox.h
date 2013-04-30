#ifndef _TEXTBOX_H_ 
#define _TEXTBOX_H_ 

#include <string>
#include "IwGxFont.h"
#include "BaseBrick.h"

class TextBox : public BaseBrick {
private:
	std::string m_sText;

	uint8 m_iAlpha;
	bool m_bAnimating;

	bool m_bShow;
	
public:
	TextBox();
	~TextBox();

	void SetText(std::string text);
	void SetVisible(bool visible);
	bool IsVisible();

private:
	bool OnUpdate(bool first);
	void OnRender(const CIwSVec2& screensize, const CIwRect& itemregion);
};

#endif
