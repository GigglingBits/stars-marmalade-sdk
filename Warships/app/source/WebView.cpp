#include "WebView.h"
#include "Configuration.h"
#include "Debug.h"
#include "IwDebug.h"

WebView::WebView() : m_pxHandle(0) {
	SetFileExtensions(Configuration::GetInstance().WebViewExts);
}

WebView::~WebView() {
	Hide();
}

bool WebView::IsShowing() {
	return NULL != m_pxHandle;
}

void WebView::Show(const CIwVec2& pos, const CIwVec2& size) {
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
			pos.x, pos.y, size.x, size.y);
	
		IwAssertMsg(MYAPP, res == S3E_RESULT_SUCCESS, ("Cannot show web view '%s'. Error: %s", GetFile().c_str(), s3eWebViewGetErrorString()));
	}
}

void WebView::Hide() {
	IW_CALLSTACK_SELF;

	if (m_pxHandle) {
		s3eWebViewHide(m_pxHandle);
    	s3eWebViewDestroy(m_pxHandle);
		m_pxHandle = NULL;
	}
}
