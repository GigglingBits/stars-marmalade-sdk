#ifndef __TOUCHPAD_H__
#define __TOUCHPAD_H__

#include "Renderable.h"

class Touchpad : public Renderable {
private:
    CIwSVec2 m_xSize;
    
public:
    Touchpad(const CIwSVec2& screensize);
    
public:
    bool HitTest(const CIwSVec2& screenpos);
    
private:
    virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
