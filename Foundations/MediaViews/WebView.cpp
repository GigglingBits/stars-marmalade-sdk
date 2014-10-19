#include "WebView.h"
#include "Debug.h"

WebView::WebView(const std::string& filename) : MediaFileView(filename),  m_pxHandle(0) {
}

WebView::~WebView() {
	Hide();
}

void WebView::Initialize() {
}

bool WebView::IsShowing() {
	return NULL != m_pxHandle;
}

void WebView::Show() {
	IW_CALLSTACK_SELF;

	Hide();
	
	if (!m_pxHandle && s3eWebViewAvailable()) {
		m_pxHandle = s3eWebViewCreate(true);
	}
	if (m_pxHandle) {
		s3eResult res = s3eWebViewNavigate(m_pxHandle, GetFile().c_str());
		IwAssertMsg(MYAPP, res == S3E_RESULT_SUCCESS, ("Cannot navigate to web view '%s'. Error: %s", GetFile().c_str(), s3eWebViewGetErrorString()));

		res = s3eWebViewShow(
			m_pxHandle, 
			GetPosition().x, GetPosition().y, GetPosition().w, GetPosition().h);
		IwAssertMsg(MYAPP, res == S3E_RESULT_SUCCESS, ("Cannot show web view '%s'. Error: %s", GetFile().c_str(), s3eWebViewGetErrorString()));
	}
}

void WebView::Hide(){
	if (m_pxHandle) {
		s3eWebViewHide(m_pxHandle);
    	s3eWebViewDestroy(m_pxHandle);
		m_pxHandle = NULL;
	}
}

void WebView::OnDoLayout(const CIwSVec2& screensize) {
	Show();
}

void WebView::OnUpdate(const FrameData& frame) {
//	MediaFileView::OnUpdate(frame);
}

void WebView::OnRender(Renderer& renderer, const FrameData& frame) {
//	MediaFileView::OnRender(renderer, frame);
}
