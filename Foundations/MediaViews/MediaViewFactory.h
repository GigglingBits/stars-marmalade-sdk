#ifndef __MEDIAVIEWFACTORY_H__
#define __MEDIAVIEWFACTORY_H__

#include "MediaView.h"

class MediaViewFactory {
public:
	static MediaView* CreateViewForFile(const std::string& filename);
	static MediaView* CreateViewForTexture(Texture* texture);
	
private:
	static std::string ToLower(const std::string& s);
};

#endif
