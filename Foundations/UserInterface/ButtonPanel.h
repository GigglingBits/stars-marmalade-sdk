#ifndef __BUTTONPANEL_H__
#define __BUTTONPANEL_H__

#include "Window.h"
#include "Button.h"
#include "Curtain.h"

#define BTNPANEL_MAX_BTN_COUNT 5

class ButtonPanel : public Window {
private:
	Curtain m_xCurtain;

	Button m_xToggle;
	Button* m_apxButtons[BTNPANEL_MAX_BTN_COUNT];
	int m_iButtonCount;

	bool m_bEnabled;
	bool m_bPanelVisible;

	VertexStreamScreen m_xPanelArea;

public:
	ButtonPanel(ButtonCommandId cmdid, s3eKey key, long userdata = 0);
	virtual ~ButtonPanel();

	void Initialize();

	bool IsPanelVisible();
	void ShowPanel();

	bool IsEnabled();
	void SetEnabled(bool enabled);

	Button& GetMainButton();

private:
	void SetButtonsEnabled(bool enabled);
	void PressedEventHandler(const Button& sender, const Button::EventArgs& args);

protected:
	void AddButton(Button& button);

	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);

public:	
	struct EventArgs {
		bool IsOpen;
	};
	Event<ButtonPanel, EventArgs> StateChanged;
};

#endif
