#ifndef __BUTTONPANEL_H__
#define __BUTTONPANEL_H__

#include "Window.h"
#include "Button.h"
#include "Curtain.h"

#define BTNPANEL_MAX_BTN_COUNT 5

class ButtonPanel : public Window {
private:
	bool m_bPanelEnabled;
	bool m_bPanelOpen;

	VertexStreamScreen m_xPanelArea;

	Button m_xToggle;
	Button* m_apxButtons[BTNPANEL_MAX_BTN_COUNT];
	int m_iButtonCount;

	Curtain m_xCurtain;

public:
	ButtonPanel(ButtonCommandId cmdid, s3eKey key, long userdata = 0);
	virtual ~ButtonPanel();

	void Initialize();

	void OpenPanel();
	bool IsPanelOpen();

	void SetEnabled(bool enabled);
	bool IsEnabled();

	Button& GetMainButton();

private:
	void SetButtonsEnabled(bool enabled);
	void PressedEventHandler(const Button& sender, const Button::EventArgs& args);

protected:
	void SetAlpha(uint32 alpha);
	void SetFadeTime(uint32 fadetime);
	
	void AddButton(Button& button);

	int GetButtonCount();
	Button* GetButton(int position);
	
	
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);

public:	
	struct EventArgs {
		bool IsPanelOpen;
	};
	MulticastEvent<ButtonPanel, EventArgs> StateChanged;
};

#endif
