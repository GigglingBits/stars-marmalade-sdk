#ifndef __LEVELHUDTEXT_H__
#define __LEVELHUDTEXT_H__

#include "Window.h"
#include "Texture.h"

class LevelHudText : public Window {
private:
	CIwSVec2 m_xPosition;
	CIwSVec2 m_xSize;

	Renderer::FontType m_eFont;
	uint32 m_uiColour;
	
	std::string m_sText;

	VertexStreamScreen m_xBackgroundShape;
	Texture* m_pxBackground;
	
public:
	LevelHudText();
	virtual ~LevelHudText();

	virtual void Initialize();

	void SetPosition(const CIwSVec2 pos);
	void SetSize(const CIwSVec2 size);
	
	void SetFont(Renderer::FontType font);
	void SetColour(uint32 col);

	void SetText(const std::string& s);

	void SetBackground(const std::string& texturename);

protected:
	virtual void OnDoLayout(const CIwSVec2& screensize);
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	
private:
	void SetBackgroundShape();
};

#endif
