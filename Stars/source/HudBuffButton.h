#ifndef __HUDBUFFBUTTON_H__
#define __HUDBUFFBUTTON_H__

#include "ButtonEx.h"

class HudBuffButton : public ButtonEx {
private:
	float m_fFillPercent;
	std::string m_sSkin;
	
public:
	HudBuffButton(const std::string& skin, ButtonCommandId cmdid, s3eKey key);

	void SetFillDegree(float n);
	
protected:
	virtual void OnTextureLoaded(Texture& texture);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
