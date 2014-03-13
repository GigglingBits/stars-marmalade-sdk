#include "VideoView.h"
#include "Debug.h"

VideoView::VideoView(const std::string& filename) : MediaFileView(filename) {
	m_bStopCallbackRegistered = S3E_RESULT_SUCCESS == s3eVideoRegister(S3E_VIDEO_STOP, (s3eCallback)OnVideoStopped, this);
}

VideoView::~VideoView() {
	if (m_bStopCallbackRegistered) {
		s3eVideoUnRegister(S3E_VIDEO_STOP, (s3eCallback)OnVideoStopped);
	}

	s3eVideoStop();
}

void VideoView::Initialize() {
	//Play();
}

bool VideoView::IsPlaying() {
	s3eVideoStatus state = (s3eVideoStatus)s3eVideoGetInt(S3E_VIDEO_STATUS);
	return state == S3E_VIDEO_PLAYING || state == S3E_VIDEO_PAUSED;
}

void VideoView::Play() {
	IW_CALLSTACK_SELF;

	s3eVideoStop();
	
	s3eResult res = s3eVideoPlay(
		GetFile().c_str(), 1, 
		GetPosition().x, GetPosition().y, GetPosition().w, GetPosition().h);

	if (res != S3E_RESULT_SUCCESS) {
		IwAssertMsg(MYAPP, false, ("Cannot play video '%s'. Error: %s", GetFile().c_str(), s3eVideoGetErrorString()));
	}
}

int32 VideoView::OnVideoStopped(void* systemData, void* userData) {
	if (VideoView* view = (VideoView*)userData) {
		int dummyarg = 0;
		view->Finished.Invoke(*view, dummyarg);
	    return true;
	}
    return false;
}

void VideoView::OnDoLayout(const CIwSVec2& screensize) {
	Play();
}

void VideoView::OnUpdate(const FrameData& frame) {
	//	MediaFileView::OnUpdate(frame);
}

void VideoView::OnRender(Renderer& renderer, const FrameData& frame) {
	//	MediaFileView::OnRender(renderer, frame);
}

