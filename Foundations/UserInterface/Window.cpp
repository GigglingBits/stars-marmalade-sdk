#include "Window.h"
#include "Debug.h"
#include "DeviceInfo.h"

Window::Window() : m_xScreenSize(CIwSVec2::g_Zero), m_bIsLayoutDone(false), m_pxResGroup(NULL) {
	m_iScreenPpcm = DeviceInfo::GetInfo().GetScreenPpcm();
}

Window::~Window() {
	if (m_pxResGroup) {
		IwGetResManager()->DestroyGroup(m_pxResGroup);
	}
}

void Window::InvalidateLayout() {
	m_bIsLayoutDone = false;
}

void Window::LoadResources(const std::string& grpname) {
	if (m_pxResGroup) {
		IwAssertMsg(MYAPP, false, ("Failed to load resource group '%s'. Another group is already associated. The Window class can only be associated to one resource group.", grpname.c_str()));
		IwGetResManager()->DestroyGroup(m_pxResGroup);
	}
	m_pxResGroup = IwGetResManager()->LoadGroup(grpname.c_str());
}

void Window::Update(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (!(m_bIsLayoutDone && frame.GetScreensize() == m_xScreenSize)) {
		m_xScreenSize = frame.GetScreensize();
		OnDoLayout(m_xScreenSize);
		m_bIsLayoutDone = true;
	}

	Renderable::Update(frame);
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
