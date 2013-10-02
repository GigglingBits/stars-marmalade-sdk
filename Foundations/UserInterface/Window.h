#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Renderable.h"
#include "IwResManager.h"

class Window : public Renderable {
private:
	int m_iScreenPpcm;
	CIwSVec2 m_xScreenSize;
	
	bool m_bIsLayoutDone;

	CIwResGroup* m_pxResGroup;
	
public:
	Window();
	virtual ~Window();

	virtual void Initialize() = 0;
	virtual void Update(const FrameData& frame);

	void InvalidateLayout();
	
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

protected:
	void LoadResources(const std::string& grpname);
};

#endif
