#include "TextureFramed.h"

#include "IwResManager.h"
#include "Renderer.h"
#include "Debug.h"


TextureFramed::TextureFramed(const TextureTemplate& texturedef) : m_rxCurrentFrame(m_xNullFrame) {
	m_bHorizontalFlip = false;
	m_iFrameElapsed = 0;
	
	TextureTemplate tpl(texturedef);
	LoadFrames(tpl);
	
	// set the starting picture
	SelectFirstFrame();
}

bool TextureFramed::IsImage() {
	return (m_rxCurrentFrame.type == eFrameTypeImage || m_rxCurrentFrame.type == eFrameTypeImageAnimation) && m_rxCurrentFrame.image;
}

CIwTexture* TextureFramed::GetImage() {
	IW_CALLSTACK_SELF;
	IwAssert(MYAPP, IsImage());
	return m_rxCurrentFrame.image;
}

bool TextureFramed::IsPattern() {
	return m_rxCurrentFrame.type == eFrameTypePattern && m_rxCurrentFrame.image;
}

CIwTexture* TextureFramed::GetPattern() {
	IW_CALLSTACK_SELF;
	IwAssert(MYAPP, IsPattern());
	return m_rxCurrentFrame.image;
}

bool TextureFramed::IsColour() {
	return !IsImage() && !IsPattern() && !IsSkeleton();
}

uint32 TextureFramed::GetColour() {
	IW_CALLSTACK_SELF;
	IwAssert(MYAPP, IsColour());
	return m_rxCurrentFrame.colour;
}

bool TextureFramed::ContainsFrame(const std::string name) {
	for (FrameList::iterator i = m_xFrames.begin(); i != m_xFrames.end(); i++) {
		if (i->id == name) {
			return true;
		}
	}
	return false;
}

void TextureFramed::LoadFrames(TextureTemplate& texturedef) {
	IW_CALLSTACK_SELF;
	
	// load list of images
	int count = texturedef.GetFrameCount();
	for (int i = 0; i < count; i++) {
		const TextureFrame& frametpl = texturedef.GetFrameInfo(i);
		
		InternalFrame frame;
		frame.type = frametpl.type;
		frame.id = frametpl.id;
		frame.healthlevel = frametpl.healthlevel;
		frame.imageresource = frametpl.imageresource;
		frame.colour = frametpl.colour;
		frame.duration = frametpl.duration;
		frame.nextid = frametpl.nextid;
		
		if (frame.type == eFrameTypeImage || frame.type == eFrameTypeImageAnimation || frame.type == eFrameTypePattern) {
			frame.image = (CIwTexture*)IwGetResManager()->GetResNamed(frame.imageresource.c_str(), "CIwTexture");
			IwAssertMsg(MYAPP, frame.image, ("Could not load texture '%s' because it seems to be missing in the resource file.", frame.imageresource.c_str()));
			if (!frame.image) {
				frame.image = (CIwTexture*)IwGetResManager()->GetResNamed("image_missing", "CIwTexture");
				IwAssertMsg(MYAPP, frame.image, ("Could not load texture '%s' because it seems to be missing in the resource file.", "image_missing"));
			}
		}
		m_xFrames.append(frame);
	}
}

bool TextureFramed::SelectFirstFrame() {
	// reset animation
	m_iFrameElapsed = 0;
	
	if (m_xFrames.num_p > 0) {
		m_rxCurrentFrame = m_xFrames.element_at(0);
		return true;
	}
	
	// if no images, set the default image
	m_rxCurrentFrame = m_xNullFrame;
	return false;
}

bool TextureFramed::SelectFrame(const std::string& id) {
	// select frame by name (max health)
	return SelectFrame(id, 100);
}

bool TextureFramed::ReSelectFrame(int health) {
	// re-select same frame, but with new health value
	return SelectFrame(m_rxCurrentFrame.id, health);
}

bool TextureFramed::SelectFrame(const std::string& id, int health) {
	// reset animation
	m_iFrameElapsed = 0;
	
	FrameList::iterator candidate = m_xFrames.end();
	long candidatehealthoffset = 0x7fffffff; // just something incredibly big, so we can tune in
	
	// look for best match
	for (FrameList::iterator it = m_xFrames.begin(); it != m_xFrames.end(); it++) {
		InternalFrame& item = *it;
		if (!item.id.compare(id)) {
			long healthoffset = std::abs((long)item.healthlevel - health);
			if (healthoffset < candidatehealthoffset) {
				candidate = it;
				candidatehealthoffset = healthoffset;
			}
		}
	}
	
	// employ candidate, if any
	if (candidate != m_xFrames.end()) {
		m_rxCurrentFrame = *candidate;
		return true;
	}
	
	// else, set the default image
	m_rxCurrentFrame = m_xNullFrame;
	return false;
}

std::string TextureFramed::GetCurrentFrame()  {
	return m_rxCurrentFrame.id;
}

void TextureFramed::SetHorizontalFlip(bool flip) {
	m_bHorizontalFlip = flip;
}

bool TextureFramed::GetHorizontalFlip() {
	return m_bHorizontalFlip;
}

void TextureFramed::Update(uint16 timestep) {
	// progress the animations
	if (m_rxCurrentFrame.type == eFrameTypeImageAnimation) {
		m_iFrameElapsed += timestep;
		if (m_iFrameElapsed >= m_rxCurrentFrame.duration) {
			m_iFrameElapsed -= m_rxCurrentFrame.duration;
			SelectFrame(m_rxCurrentFrame.nextid);
			//IwDebugTraceLinePrintf("Animated: Switched to frame '%s'", m_rxCurrentFrame.nextid.c_str());
		}
	} else {
		m_iFrameElapsed = 0;
	}
}
