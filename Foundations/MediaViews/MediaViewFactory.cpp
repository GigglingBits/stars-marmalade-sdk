#include "MediaViewFactory.h"

#include "Debug.h"
#include "WebView.h"
#include "VideoView.h"
#include "TextureView.h"
#include "FactoryManager.h"
#include "Debug.h"

#include <algorithm>

MediaView* MediaViewFactory::CreateViewForFile(const std::string& filename) {
	IW_CALLSTACK_SELF;
	
	MediaFileView* mv = NULL;
	
	std::string extension = ToLower(filename.substr(filename.find_last_of(".") + 1));
	if (extension == "htm" || extension == "html") {
		mv = new WebView(filename);
	
	} else if (extension == "m4v" || extension == "mp4" || extension == "jpg") {
		mv = new VideoView(filename);

	} else {
		IwAssertMsg(MYAPP, false, ("Cannot itentify player for file '%s'; the file type is not mapped.", filename.c_str()));
	}
	
	return mv;
}

MediaView* MediaViewFactory::CreateViewForTexture(const std::string& textureid) {
	TextureFactory& factory = FactoryManager::GetTextureFactory();

	TextureView* tv = NULL;
	
	if (factory.ConfigExists(textureid)) {
		tv = new TextureView(textureid);

	} else {
		IwAssertMsg(MYAPP, false, ("Cannot create view for texture '%s'; the texture cannot be found.", textureid.c_str()));
	}
	
	return tv;
}

std::string MediaViewFactory::ToLower(const std::string& s) {
	std::string lower(s);
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	return lower;
}