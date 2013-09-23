#ifndef __VIDEOVIEW_H__
#define __VIDEOVIEW_H__

#include <string>
#include "s3eVideo.h"
#include "MediaFileView.h"

class VideoView : public MediaFileView {
private:
	bool m_bStopCallbackRegistered;

public:
	VideoView(const std::string& filename);
	virtual ~VideoView();

	virtual void Initialize();

	bool IsPlaying();
	void Play();
	
protected:
	virtual void OnDoLayout(const CIwSVec2& screensize);
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	
private:
	static int32 OnVideoStopped(void* systemData, void* userData);
};

#endif
