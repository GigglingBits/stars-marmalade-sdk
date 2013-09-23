#ifndef __MEDIAFILEVIEW_H__
#define __MEDIAFILEVIEW_H__

#include "MediaView.h"

class MediaFileView : public MediaView {
private:
	std::string m_sFile;

public:
	MediaFileView(const std::string& filename);
	
protected:
	const std::string& GetFile();
};

#endif
