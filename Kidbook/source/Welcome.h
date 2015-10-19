#ifndef _WELCOME_H_ 
#define _WELCOME_H_ 

#include <string>
#include "BaseBrick.h"
#include "IwUI.h"

class Welcome : public BaseBrick {
private:
	CIwUILabel* m_pxLabel;
	CIwUIScrollableView* m_pxScrollView;

public:
	Welcome();
	virtual ~Welcome();

	void ScrollDown();
	void ScrollUp();

private:
	bool OnUpdate(bool first);
	void OnRender(const CIwSVec2& screensize, const CIwRect& itemregion);

	std::string ReadHelpText();
	void SetupControls(const char* text);
};

#endif
