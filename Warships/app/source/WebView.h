#ifndef __WEBVIEW_H__
#define __WEBVIEW_H__

#include <string>
#include "s3eWebView.h"
#include "MediaView.h"

class WebView : public MediaView {
private:
	s3eWebView* m_pxHandle;

public:
	WebView();
	virtual ~WebView();

	virtual bool IsShowing();

	virtual void Show(const CIwVec2& pos, const CIwVec2& size);
	virtual void Hide();
};

#endif
