#ifndef __PANEL_H__
#define __PANEL_H__

#include "Window.h"
#include "Curtain.h"

class Panel : public Window {
private:
	bool m_bPanelEnabled;
	bool m_bPanelOpen;

	VertexStreamScreen m_xPanelArea;
	Curtain m_xCurtain;

public:
	Panel();
	virtual ~Panel();

	void Initialize();

	void OpenPanel();
	bool IsPanelOpen();

protected:
	void SetAlpha(uint32 alpha);
	void SetFadeTime(uint32 fadetime);
	
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);
};

#endif
