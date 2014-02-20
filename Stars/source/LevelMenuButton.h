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
	
public:
	LevelMenuButton();
	
	void SetTextures(const std::string& face, const std::string& stars);
	void SetLevel(int level, bool available, bool open, int stars);

protected:
	virtual void OnStateChanged(bool enabled, bool pressed);
	
private:
	void SetBehavior(bool enabled);
};

#endif
