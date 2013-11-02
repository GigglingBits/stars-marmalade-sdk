#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Renderable.h"

#define WINDOW_NUM_COLOURS 4

class Window : public Renderable {
private:
	int m_iScreenPpcm;
	CIwSVec2 m_xScreenSize;
	CIwRect m_xPosition;

	bool m_bIsInitialPositionSet;
	bool m_bIsLayoutDone;

	VertexStreamScreen m_xBackgroundShape;
	Texture* m_pxBackgroundTexture;
	uint32* m_puiBackgroundColourStream;
	
public:
	Window();
	~Window();
	
	virtual void Initialize() = 0;
	virtual void Update(const FrameData& frame);
	virtual void Render(Renderer& renderer, const FrameData& frame);

	void InvalidateLayout();

	void SetSize(int w, int h);
	void SetPosition(int x, int y);
	void SetPosition(const CIwRect& pos);
	
	const CIwRect& GetPosition();
	
	void SetBackground(Texture* texture);
	void SetBackground(uint32 blcolour, uint32 brcolour, uint32 urcolour, uint32 ulcolour);
	void ClearBackground();
	
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
	
private:
	void SetBackgroundShape();
};

#endif
