#ifndef __LEVELSTATSPANEL_H__
#define __LEVELSTATSPANEL_H__

#include <map>

#include "Window.h"
#include "Texture.h"
#include "Button.h"
#include "Event.h"

class LevelStatsPanel : public Window {
private:
	typedef std::map<std::string, Button*> HeroMap;
	HeroMap m_xHeroMap;	

	Button& m_rxSelected;
	Button m_xNullButton;

	VertexStreamScreen m_xBackgroundShape;
	Texture* m_pxBackground;

public:
	LevelStatsPanel();
	virtual ~LevelStatsPanel();

	void Initialize();

	void SetEnabled(bool enabled);

	void Kill(const std::string& id);
	void Rescue(const std::string& id);
	void Select(const std::string& id);

	void Add(const std::string& id);

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnDoLayout(const CIwSVec2& screensize);

private:
	void ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args);

public:
	Event<LevelStatsPanel, std::string> HeroFocusRequested;
};

#endif
