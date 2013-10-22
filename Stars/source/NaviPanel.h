#ifndef __NAVIPANEL_H__
#define __NAVIPANEL_H__

#include "Button.h"
#include "Window.h"

#define NAVIPANEL_BUTTON_MAX_COUNT 5
#define NAVIPANEL_BUTTON_ON "on"
#define NAVIPANEL_BUTTON_OFF "off"

class NaviPanel : public Window {
private:
	int m_iButtonCount;
	Button* m_apxButtons[NAVIPANEL_BUTTON_MAX_COUNT];
	
public:
	NaviPanel();
	virtual ~NaviPanel();
	virtual void Initialize();

	void AddButton(const std::string& textureid, int buttonid);
	void ActivateButton(int buttonid);

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);
};

#endif
