#ifndef __HUDTEXT_H__
#define __HUDTEXT_H__

#include "Window.h"

class HudText : public Window {
private:
	Renderer::FontType m_eFont;
	uint32 m_uiColour;
	Renderer::TextAlignmentHorizontal m_eAlignH;
	Renderer::TextAlignmentVertical m_eAlignV;
	
	std::string m_sText;
	
public:
	HudText();

	virtual void Initialize();

	void SetFont(Renderer::FontType font);
	void SetColour(uint32 col);

	void SetText(const std::string& s);

	void SetAlignH(Renderer::TextAlignmentHorizontal align);
	void SetAlignV(Renderer::TextAlignmentVertical align);

	void SetBackground(const std::string& texturename);

protected:
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnUpdate(const FrameData& frame);
};

#endif
