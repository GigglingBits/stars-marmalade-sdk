#ifndef __LEVELMENUBUTTON_H__
#define __LEVELMENUBUTTON_H__

#include "ButtonEx.h"

class LevelMenuButton : public ButtonEx {
private:
	std::string m_sLevel;
	bool m_bLevelAvailable;
	bool m_bLevelOpen;
	int m_iAchievedStars;
	
	bool m_bReentrant;
	
	Texture* m_pxStars;
	
public:
	LevelMenuButton();
	
	void SetTextures(const std::string& facetexture, const std::string& starstexture);
	void SetLevel(int level, bool available, bool open, int stars);

protected:
	virtual void OnStateChanged(bool enabled, bool pressed);
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	
private:
	void SetBehavior(bool enabled);
};

#endif
