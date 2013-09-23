#ifndef __MEDIAVIEW_H__
#define __MEDIAVIEW_H__

#include <string>
#include "Window.h"
#include "Event.h"

class MediaView : public Window {
private:
	CIwVec2 m_xSize;
	CIwVec2 m_xPosition;
	
public:
	MediaView();
	
	void SetPosition(const CIwVec2& pos, const CIwVec2& size);
	
protected:
	const CIwVec2& GetPosition();
	const CIwVec2& GetSize();
	
public:
	Event<MediaView, int> Finished;
};

#endif
