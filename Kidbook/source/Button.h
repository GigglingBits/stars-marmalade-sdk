#ifndef _BUTTON_H_ 
#define _BUTTON_H_ 

#include "Iw2D.h"
#include "BaseBrick.h"
#include "IwTypes.h"

#define BTN_ID_SPEECH	0
#define BTN_ID_SOUND	1
#define BTN_ID_TEXT		2
#define BTN_ID_EXIT		3
#define BTN_ID_HELP		4

class Button : public BaseBrick {
private:
	bool m_bEnabled;
	uint8 m_iId;

	CIw2DImage* m_pxFaceEnabled;
	CIw2DImage* m_pxFaceDisabled;

	CIwRect m_xFaceRegion;

public:
	Button(uint8 id);
	virtual ~Button();

	uint8 GetId();

	void Enable(bool enable);
	bool HitTest(CIwSVec2 pos);

private:
	bool OnUpdate(bool first);
	void OnRender(const CIwSVec2& screensize, const CIwRect& itemregion);
};

#endif
