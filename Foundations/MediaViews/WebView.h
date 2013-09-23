#ifndef __WEBVIEW_H__
#define __WEBVIEW_H__

#include <string>
#include "s3eWebView.h"
#include "MediaFileView.h"

class WebView : public MediaFileView {
private:
	s3eWebView* m_pxHandle;

public:
	WebView(const std::string& filename);
	virtual ~WebView();

	virtual void Initialize();

private:
	bool IsShowing();
	void Show();
	void Hide();
	
protected:
	virtual void OnDoLayout(const CIwSVec2& screensize);
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
