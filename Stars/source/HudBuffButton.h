#ifndef __HUDBUFFBUTTON_H__
#define __HUDBUFFBUTTON_H__

#include "Button.h"

class HudBuffButton : public Button {
private:
	int m_iFillPercent;
	std::string m_sSkin;
	
public:
	HudBuffButton(const std::string& skin, ButtonCommandId cmdid, s3eKey key);

	void SetCount(int count);
	
private:
	void UpdateBehavior();
	
protected:
	virtual void OnTextureLoaded(Texture& texture);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
