#ifndef _TOOLBAR_H_ 
#define _TOOLBAR_H_ 

#include <string>
#include "Iw2d.h"

#include "Page.h"
#include "Button.h"
#include "BaseBrick.h"

class Toolbar : public BaseBrick {
private:
	CIwList<Button*> m_xButtons;

public:
	~Toolbar();

	void Reset();
	void AddButton(uint8 id);
	
	int TouchButton(CIwSVec2 pos);
	void EnableButton(uint8 id, bool enabled);
	
	int GetButtonCount();

private:
	bool OnUpdate(bool first);
	void OnRender(const CIwSVec2& screensize, const CIwRect& itemregion);
};

#endif
