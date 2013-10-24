#include "Window.h"
#include "Debug.h"
#include "DeviceInfo.h"

Window::Window() : m_xScreenSize(CIwSVec2::g_Zero), m_bIsLayoutDone(false), m_pxBackground(NULL), m_eBackgroundRenderingLayer(Renderer::eRenderingLayerBackground) {
	m_iScreenPpcm = DeviceInfo::GetInfo().GetScreenPpcm();
}

Window::~Window() {
	if (m_pxBackground) {
		delete m_pxBackground;
	}
}

void Window::SetSize(int w, int h) {
	if (w != m_xPosition.w || h != m_xPosition.h) {
		m_xPosition.w = w;
		m_xPosition.h = h;
		InvalidateLayout();
	}
}

void Window::SetPosition(int x, int y) {
	if (x != m_xPosition.x || y != m_xPosition.y) {
		m_xPosition.x = x;
		m_xPosition.y = y;
		InvalidateLayout();
	}
}

void Window::SetPosition(const CIwRect& pos) {
	if (m_xPosition.x != pos.x ||
		m_xPosition.y != pos.y ||
		m_xPosition.w != pos.w ||
		m_xPosition.h != pos.h) {
		m_xPosition = pos;
		InvalidateLayout();
	}
}

const CIwRect& Window::GetPosition() {
	return m_xPosition;
}

void Window::SetBackground(Texture* texture, Renderer::RenderingLayer layer) {
	if (m_pxBackground) {
		delete m_pxBackground;
	}
	m_eBackgroundRenderingLayer = layer;
	m_pxBackground = texture;
	if (m_pxBackground) {
		SetBackgroundShape();
	}
}

void Window::InvalidateLayout() {
	m_bIsLayoutDone = false;
}

void Window::Update(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (!(m_bIsLayoutDone && frame.GetScreensize() == m_xScreenSize)) {
		m_xScreenSize = frame.GetScreensize();
		OnDoLayout(m_xScreenSize);
		m_bIsLayoutDone = true;
		if (m_pxBackground) {
			SetBackgroundShape();
		}
	}

	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
	}

	Renderable::Update(frame);
}

void Window::Render(Renderer& renderer, const FrameData& frame) {
	if (m_pxBackground) {
		renderer.SetRederingLayer(m_eBackgroundRenderingLayer);
		renderer.Draw(m_xBackgroundShape, *m_pxBackground);
		renderer.SetDefaultRederingLayer();
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
