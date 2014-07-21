#include "VideoView.h"
#include "Debug.h"

#include "IwUtil.h"

#include "s3eFile.h"

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
	
	std::string videofile = PrepareVideoFile();
	IwTrace(MYAPP, ("Playing video from location: %s", videofile.c_str()));

	s3eResult res = s3eVideoPlay(
		videofile.c_str(), 1,
		GetPosition().x, GetPosition().y, GetPosition().w, GetPosition().h);

	if (res != S3E_RESULT_SUCCESS) {
		IwAssertMsg(MYAPP, false, ("Cannot play video '%s'. Error: %s", videofile.c_str(), s3eVideoGetErrorString()));
	}
}

std::string VideoView::PrepareVideoFile() {
	/* 
	 video file can only played form afile system that is accessible by the OS
	 */
	
	/*
	 From Marmalade doc: The following standard drives can exist. We select the preferred drive, which is
	 accessible by the os, and which are appropriate for storing decompressed video files.
	 
	 'rom' for the read-only portion of the s3e data area.
	 'ram' for the writable portion of the s3e data area.
	 'rst' for a removable memory card.
	 'raw' for paths that will be passed directly to the underlying operating system without modification.
	 'tmp' for a system temp folder, outside of the s3e data area.
	 'cache' for an app-specific data folder that is marked not to backed up by the system.
	 */
	
	std::string file;
	file = std::string("ram://").append(GetFile());
	if (s3eFileCheckExists(file.c_str())) {
		// accessible out of the box (since it us writable, it cannot be in the derbh file)
		IwTrace(MYAPP, ("Found video file here: %s", file.c_str()));
		return file;
	}
	
	file = std::string("rom://").append(GetFile());
	if (s3eFileCheckExists(file.c_str())) {
		// accessible out of the box (not writable; but I ASSUME that derbh-files under the
		// auto-derbh option are not mounted under rom:://)
		IwTrace(MYAPP, ("Found video file here: %s", file.c_str()));
		return file;
	}
	
	file = std::string("cache://").append(GetFile());
	if (s3eFileCheckExists(file.c_str())) {
		// must already have been have been copied there previously
		IwTrace(MYAPP, ("Found video file here: %s", file.c_str()));
		return file;
	}
	
	file = std::string("tmp://").append(GetFile());
	if (s3eFileCheckExists(file.c_str())) {
		// must have been copied there previously
		IwTrace(MYAPP, ("Found video file here: %s", file.c_str()));
		return file;
	}
	
	// can s3e fine the faile in anyother place?
	// likely in the derbh-file due to the auto-derbh option?
	// copy the file to a place where we know that the OS can read it
	file = GetFile();
	if (s3eFileCheckExists(file.c_str())) {
		std::string targetfile = GetFile();
		
		// flatten directory structure, so that we don't need to
		// create any directories prior to copying
		std::replace(targetfile.begin(), targetfile.end(), '/', '_');
		std::replace(targetfile.begin(), targetfile.end(), '\\', '_');

		if (CheckDriveExists("cache://")) {
			targetfile = std::string("cache://").append(targetfile);
		} else if (CheckDriveExists("tmp://")) {
			targetfile = std::string("tmp://").append(targetfile);
		} else {
			targetfile = std::string("ram://").append(targetfile);
		}
		IwTrace(MYAPP, ("Copying video file form '%s' to '%s'", file.c_str(), targetfile.c_str()));
		if (S3E_RESULT_SUCCESS == IwFileCopy(targetfile.c_str(), file.c_str())) {
			return targetfile;
		} else {
			IwError(("Could not copy video file form '%s' to '%s'", file.c_str(), targetfile.c_str()));
		}
	}
	
	return GetFile();
}

bool VideoView::CheckDriveExists(const std::string& drivename) {
	s3eFileList* list = s3eFileListDirectory(NULL);
	const int buflen = 256;
    char buffer[buflen];
	bool found = false;
	
    while (!found && s3eFileListNext(list, buffer, buflen) != S3E_RESULT_ERROR) {
		IwTrace(MYAPP, ("Found drive '%s'", buffer));
		found = !strncmp(buffer, drivename.c_str(), std::min<int>(buflen, drivename.size()));
    }
	s3eFileListClose(list);

	return found;
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

