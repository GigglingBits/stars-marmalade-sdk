#ifndef __MEDIAVIEWFACTORY_H__
#define __MEDIAVIEWFACTORY_H__

#include "MediaView.h"

class MediaViewFactory {
public:
	static MediaView* CreateViewForFile(const std::string& filename);
	static MediaView* CreateViewForTexture(const std::string& textureid);
	
private:
	static std::string ToLower(const std::string& s);
};

#endif
