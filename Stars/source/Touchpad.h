#ifndef __TOUCHPAD_H__
#define __TOUCHPAD_H__

#include "Renderable.h"

class Touchpad : public Renderable {
private:
	CIwSVec2 m_xCenterPos;	// touchpad position in screenspace
	CIwSVec2 m_xSize;		// touchpad size in screenspace

	CIwSVec2 m_xTouchPos;	// position of the touch

public:
    Touchpad();

    void SetPosition(const CIwSVec2& center);
    void SetSize(const CIwSVec2& size);

	bool SetTouch(const CIwSVec2& screenpos);

	CIwFVec2 GetTouchVectorNormalized();
	
private:
    bool HitTest(const CIwSVec2& screenpos);
    
private:
    virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
