#include "MediaViewFactory.h"

#include "Debug.h"
#include "WebView.h"
#include "VideoView.h"
#include "TextureView.h"
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

MediaView* MediaViewFactory::CreateViewForTexture(Texture* texture) {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, texture, ("Cannot create view for NULL texture"));
	
	if (texture) {
		return new TextureView(texture);
	}
	return NULL;
}

std::string MediaViewFactory::ToLower(const std::string& s) {
	std::string lower(s);
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	return lower;
}