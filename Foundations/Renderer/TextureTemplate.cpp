#include "TextureTemplate.h"
#include "IwDebug.h"

void TextureTemplate::AddFrame(const std::string& id, int healthlevel, const std::string& image, const std::string& pattern, uint32 colour, int duration, const std::string& nextid) {
	if (!image.empty()) {
		if (duration != 0 && !nextid.empty()) {
			AddAnimation(id, healthlevel, image, duration, nextid);
		} else if (duration == 0 && nextid.empty()) {
			AddImage(id, healthlevel, image);
		} else {
			IwAssertMsg(MYAPP, false, ("Frame definition '%s' in texture is invalid. If a duration is defined, also the 'next' must be defined. And vice versa.", id.c_str()));
		}
	} else if (!pattern.empty()) {
		IwAssertMsg(MYAPP, duration == 0 && nextid.empty(), ("Frame definition '%s' in texture is invalid. Neither 'duration' nor 'nextid' must be defined for pattern frames.", id.c_str()));
		AddPattern(id, healthlevel, pattern);
	} else {
		IwAssertMsg(MYAPP, duration == 0 && nextid.empty(), ("Frame definition '%s' in texture is invalid. Neither 'duration' nor 'nextid' must be defined for colour frames.", id.c_str()));
		AddColour(id, healthlevel, colour);
	}
}

void TextureTemplate::AddImage(const std::string& id, int healthlevel, const std::string& image) {
	TextureFrame frame;
	frame.type = eFrameTypeImage;
	frame.id = id;
	frame.healthlevel = healthlevel;
	frame.imageresource = image;
	m_xFrames.push_back(frame);
}

void TextureTemplate::AddPattern(const std::string& id, int healthlevel, const std::string& pattern) {
	TextureFrame frame;
	frame.type = eFrameTypePattern;
	frame.id = id;
	frame.healthlevel = healthlevel;
	frame.imageresource = pattern;
	m_xFrames.push_back(frame);
}

void TextureTemplate::AddAnimation(const std::string& id, int healthlevel, const std::string& image, int duration, const std::string& nextid) {
	TextureFrame frame;
	frame.type = eFrameTypeAnimation;
	frame.id = id;
	frame.healthlevel = healthlevel;
	frame.imageresource = image;
	frame.duration = duration;
	frame.nextid = nextid;
	m_xFrames.push_back(frame);
}

void TextureTemplate::AddColour(const std::string& id, int healthlevel, uint32 colour) {
	TextureFrame frame;
	frame.type = eFrameTypeColour;
	frame.id = id;
	frame.healthlevel = healthlevel;
	frame.colour = colour;
	m_xFrames.push_back(frame);
}

int TextureTemplate::GetFrameCount() {
	return m_xFrames.size();
}

const TextureFrame& TextureTemplate::GetFrameInfo(int index) {
	return m_xFrames[index];
}
