#include "Window.h"
#include "Debug.h"
#include "DeviceInfo.h"

Window::Window() : m_xScreenSize(CIwSVec2::g_Zero), m_xPosition(0, 0, 0, 0), m_bIsLayoutDone(false), m_bIsInitialPositionSet(false), m_pxBackgroundTexture(NULL), m_puiBackgroundColourStream(NULL) {
	m_iScreenPpcm = DeviceInfo::GetInstance().GetPixelsPerInch();
}

Window::~Window() {
	if (m_pxBackgroundTexture) {
		delete m_pxBackgroundTexture;
	}
}

void Window::SetSize(int w, int h) {
	if (w != m_xPosition.w || h != m_xPosition.h) {
		m_xPosition.w = w;
		m_xPosition.h = h;
		InvalidateLayout();
	}
	m_bIsInitialPositionSet = true;
}

void Window::SetPosition(int x, int y) {
	if (x != m_xPosition.x || y != m_xPosition.y) {
		m_xPosition.x = x;
		m_xPosition.y = y;
		InvalidateLayout();
	}
	m_bIsInitialPositionSet = true;
}

void Window::SetPosition(const CIwRect& pos) {
	if (m_xPosition.x != pos.x ||
		m_xPosition.y != pos.y ||
		m_xPosition.w != pos.w ||
		m_xPosition.h != pos.h) {
		m_xPosition = pos;
		InvalidateLayout();
	}
	m_bIsInitialPositionSet = true;
}

const CIwRect& Window::GetPosition() {
	return m_xPosition;
}

void Window::SetBackground(Texture* texture) {
	IW_CALLSTACK_SELF;

	IwAssertMsg(MYAPP, texture, ("Background should not be set to empty texture. Use ClearBackground() instead."));
	
	ClearBackground();
	m_pxBackgroundTexture = texture;
}

void Window::SetBackground(uint32 blcolour, uint32 brcolour, uint32 urcolour, uint32 ulcolour) {
	ClearBackground();

	m_puiBackgroundColourStream = new uint32[WINDOW_NUM_COLOURS];
	m_puiBackgroundColourStream[0] = blcolour;
	m_puiBackgroundColourStream[1] = brcolour;
	m_puiBackgroundColourStream[2] = urcolour;
	m_puiBackgroundColourStream[3] = ulcolour;
}

void Window::ClearBackground() {
	if (m_pxBackgroundTexture) {
		delete m_pxBackgroundTexture;
	}
	if (m_puiBackgroundColourStream) {
		delete [] m_puiBackgroundColourStream;
	}
}

void Window::InvalidateLayout() {
	m_bIsLayoutDone = false;
}

void Window::Update(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (!(m_bIsLayoutDone && frame.GetScreensize() == m_xScreenSize)) {
		m_xScreenSize = frame.GetScreensize();
		if (!m_bIsInitialPositionSet) {
			SetPosition(0, 0);
			SetSize(m_xScreenSize.x, m_xScreenSize.y);
			m_bIsInitialPositionSet = true;
		}
		OnDoLayout(m_xScreenSize);
		m_bIsLayoutDone = true;
		SetBackgroundShape();
	}

	if (m_pxBackgroundTexture) {
		m_pxBackgroundTexture->Update(frame.GetRealDurationMs());
	}

	Renderable::Update(frame);
}

void Window::Render(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;
		
	if (m_pxBackgroundTexture) {
		renderer.SetRederingLayer(GetRederingLayer());
		renderer.Draw(m_xBackgroundShape, *m_pxBackgroundTexture);
	} else if (m_puiBackgroundColourStream && sizeof(m_puiBackgroundColourStream) <= m_xBackgroundShape.GetVertCount()) {
		renderer.SetRederingLayer(GetRederingLayer());
		renderer.DrawPolygon(m_xBackgroundShape.GetVerts(), m_xBackgroundShape.GetVertCount(), m_puiBackgroundColourStream, m_puiBackgroundColourStream);
	} else if (m_puiBackgroundColourStream) {
		IwAssertMsg(MYAPP,  sizeof(m_puiBackgroundColourStream) <= m_xBackgroundShape.GetVertCount(), ("The colour stream contains more colours than the windown shape. This is likely undesired, and could indicate an error."));
	}
	
	Renderable::Render(renderer, frame);
}

int Window::GetScreenExtents() {
	int extents = std::min<int16>(
		m_xScreenSize.x, 
		m_xScreenSize.y);
	IwAssertMsg(MYAPP, extents > 0, ("Screen extents are smaller than expected (smaller or equal 0). Is the function called prematurely?"));
	return extents;
}

void Window::OnDoLayout(const CIwSVec2& screensize) {
	// override this for re-doing the page layouts
}

void Window::SetBackgroundShape() {
	m_xBackgroundShape.SetRect(GetPosition());
}
