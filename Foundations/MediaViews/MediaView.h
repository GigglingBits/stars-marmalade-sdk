#ifndef __MEDIAVIEW_H__
#define __MEDIAVIEW_H__

#include <string>
#include "Window.h"
#include "MulticastEvent.h"

class MediaView : public Window {
private:
	CIwVec2 m_xSize;
	CIwVec2 m_xPosition;
	
public:
	MediaView();
	
public:
	MulticastEvent<MediaView, int> Finished;
};

#endif
