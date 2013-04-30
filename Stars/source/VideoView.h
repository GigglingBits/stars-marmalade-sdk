#ifndef __VIDEOVIEW_H__
#define __VIDEOVIEW_H__

#include <string>
#include "s3eVideo.h"
#include "MediaView.h"

class VideoView : public MediaView {
private:
	bool m_bStopCallbackRegistered;

public:
	VideoView();
	virtual ~VideoView();

	virtual bool IsShowing();

	virtual void Show(const CIwVec2& pos, const CIwVec2& size);
	virtual void Hide();

private:
	static int32 OnVideoStopped(void* systemData, void* userData);
};

#endif
