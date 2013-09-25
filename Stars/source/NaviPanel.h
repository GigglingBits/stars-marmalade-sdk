#ifndef __NAVIPANEL_H__
#define __NAVIPANEL_H__

#include "Button.h"
#include "Window.h"

#define NAVIPANEL_MAX_BUTTONS 5

class NaviPanel : public Window {
private:
	CIwRect m_xPosition;

	int m_iButtonCount;
	Button* m_apxButtons[NAVIPANEL_MAX_BUTTONS];
	
public:
	NaviPanel();
	virtual ~NaviPanel();
	virtual void Initialize();

	void AddButton(const std::string& textureid, ButtonCommandId cmdid, s3eKey key = s3eKeyFirst, long userdata = 0);
	
	void SetPosition(const CIwRect& rect);

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);
};

#endif
