#include "VideoView.h"
#include "Configuration.h"
#include "Debug.h"
#include "IwDebug.h"

VideoView::VideoView() {
	SetFileExtensions(Configuration::GetInstance().VideoViewExts);
	m_bStopCallbackRegistered = S3E_RESULT_SUCCESS == s3eVideoRegister(S3E_VIDEO_STOP, (s3eCallback)OnVideoStopped, this);
}

VideoView::~VideoView() {
	if (m_bStopCallbackRegistered) {
		s3eVideoUnRegister(S3E_VIDEO_STOP, (s3eCallback)OnVideoStopped);
	}
	Hide();
}

bool VideoView::IsShowing() {
	s3eVideoStatus state = (s3eVideoStatus)s3eVideoGetInt(S3E_VIDEO_STATUS);
	return state == S3E_VIDEO_PLAYING || state == S3E_VIDEO_PAUSED;
}

void VideoView::Show(const CIwVec2& pos, const CIwVec2& size) {
	IW_CALLSTACK_SELF;

	Hide();
	
	s3eResult res = s3eVideoPlay(
		GetFile().c_str(), 1, 
		pos.x, pos.y, size.x, size.y);

	if (res != S3E_RESULT_SUCCESS) {
		IwAssertMsg(MYAPP, false, ("Cannot play video '%s'. Error: %s", GetFile().c_str(), s3eVideoGetErrorString()));
	}
}

void VideoView::Hide() {
	s3eVideoStop();
}

int32 VideoView::OnVideoStopped(void* systemData, void* userData) {
	if (VideoView* view = (VideoView*)userData) {
		int dummyarg = 0;
		view->Finished.Invoke(*view, dummyarg);
	    return true;
	}
    return false;
}
