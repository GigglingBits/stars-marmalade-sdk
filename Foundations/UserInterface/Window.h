#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Renderable.h"

class Window : public Renderable {
private:
	int m_iScreenPpcm;
	CIwSVec2 m_xScreenSize;
	CIwRect m_xPosition;
	
	bool m_bIsLayoutDone;
	
public:
	Window();

	virtual void Initialize() = 0;
	virtual void Update(const FrameData& frame);

	void InvalidateLayout();

	void SetSize(int w, int h);
	void SetPosition(int x, int y);
	void SetPosition(const CIwRect& pos);
	
	const CIwRect& GetPosition();
	
protected:
	int GetScreenExtents();

	virtual void OnDoLayout(const CIwSVec2& screensize);

	template<typename T>
	T PixelToCentimeter(T pixels) {
		return pixels / (T)m_iScreenPpcm;
	}
	
	template<typename T>
	T CentimeterToPixel(T cm) {
		return cm * (T)m_iScreenPpcm;
	}
};

#endif
