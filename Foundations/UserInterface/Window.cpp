#include "Window.h"
#include "Debug.h"
#include "DeviceInfo.h"

Window::Window() : m_xScreenSize(CIwSVec2::g_Zero), m_xPosition(0, 0, 0, 0), m_bIsLayoutDone(false), m_bIsInitialPositionSet(false), m_pxBackgroundTexture(NULL) {
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

void Window::ClearBackground() {
	if (m_pxBackgroundTexture) {
		delete m_pxBackgroundTexture;
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
		renderer.SetRenderingLayer(GetRederingLayer());
		renderer.Draw(m_xBackgroundShape, *m_pxBackgroundTexture);
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
