#include "TextureTemplate.h"
#include "IwDebug.h"
#include "Debug.h"

bool TextureTemplate::IsSkeletonAnimation() const {
	return !m_sSkeletonAnimation.empty();
}

const std::string& TextureTemplate::GetSkeletonAnimation() const {
	return m_sSkeletonAnimation;
}

void TextureTemplate::SetSkeletonAnimation(const std::string& animation) {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, GetFrameCount() == 0, ("Cannot add skeleton to texture. Frames already exist."));
	m_sSkeletonAnimation = animation;
}

void TextureTemplate::AddFrame(const std::string& id, int healthlevel, const std::string& image, const std::string& pattern, uint32 colour, int duration, const std::string& nextid) {
	IW_CALLSTACK_SELF;
	
	if (!image.empty()) {
		if (duration != 0 && !nextid.empty()) {
			AddImageAnimation(id, healthlevel, image, duration, nextid);
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
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, !IsSkeletonAnimation(), ("Cannot add image to skeleton textures."));
	
	TextureFrame frame;
	frame.type = eFrameTypeImage;
	frame.id = id;
	frame.healthlevel = healthlevel;
	frame.imageresource = image;
	m_xFrames.push_back(frame);
}

void TextureTemplate::AddPattern(const std::string& id, int healthlevel, const std::string& pattern) {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, !IsSkeletonAnimation(), ("Cannot add pattern to skeleton textures."));
	
	TextureFrame frame;
	frame.type = eFrameTypePattern;
	frame.id = id;
	frame.healthlevel = healthlevel;
	frame.imageresource = pattern;
	m_xFrames.push_back(frame);
}

void TextureTemplate::AddImageAnimation(const std::string& id, int healthlevel, const std::string& image, int duration, const std::string& nextid) {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, !IsSkeletonAnimation(), ("Cannot add image animation to skeleton textures."));
	
	TextureFrame frame;
	frame.type = eFrameTypeImageAnimation;
	frame.id = id;
	frame.healthlevel = healthlevel;
	frame.imageresource = image;
	frame.duration = duration;
	frame.nextid = nextid;
	m_xFrames.push_back(frame);
}

void TextureTemplate::AddColour(const std::string& id, int healthlevel, uint32 colour) {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, !IsSkeletonAnimation(), ("Cannot add colour animation to skeleton textures."));

	TextureFrame frame;
	frame.type = eFrameTypeColour;
	frame.id = id;
	frame.healthlevel = healthlevel;
	frame.colour = colour;
	m_xFrames.push_back(frame);
}

int TextureTemplate::GetFrameCount() const {
	return m_xFrames.size();
}

const TextureFrame& TextureTemplate::GetFrameInfo(int index) const {
	return m_xFrames[index];
}
