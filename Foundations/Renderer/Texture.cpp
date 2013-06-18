#include "Texture.h"
#include "IwResManager.h"
#include "Renderer.h"
#include "Debug.h"

Texture::Texture(const TextureTemplate& texturedef) : m_rxCurrentFrame(m_xNullFrame) {
	m_bHorizontalFlip = false;
	m_iFrameElapsed = 0;

	TextureTemplate tpl(texturedef);
	LoadFrames(tpl);

	// set the starting picture
	SelectFirstFrame();
}

bool Texture::IsImage() {
	return (m_rxCurrentFrame.type == eFrameTypeImage || m_rxCurrentFrame.type == eFrameTypeAnimation) && m_rxCurrentFrame.image;
}

CIwTexture* Texture::GetImage() {
	if (IsImage()) {
		return m_rxCurrentFrame.image;
	}
	return NULL;
}

bool Texture::IsPattern() {
	return m_rxCurrentFrame.type == eFrameTypePattern && m_rxCurrentFrame.image;
}

CIwTexture* Texture::GetPattern() {
	if (IsPattern()) {
		return m_rxCurrentFrame.image;
	}
	return NULL;
}

bool Texture::IsColour() {
	return !IsImage() && !IsPattern();
}

uint32 Texture::GetColour() {
	if (IsColour()) {
		return m_rxCurrentFrame.colour;
	}
	return 0;
}

bool Texture::ContainsFrame(const std::string name) {
	for (FrameList::iterator i = m_xFrames.begin(); i != m_xFrames.end(); i++) {
		if (i->id == name) {
			return true;
		}
	}
	return false;
}

void Texture::LoadFrames(TextureTemplate& texturedef) {
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
		frame.duration = frametpl.duration;
		frame.nextid = frametpl.nextid;

		if (frametpl.type == eFrameTypeImage || frametpl.type == eFrameTypeAnimation || frametpl.type == eFrameTypePattern) {
			CIwResGroup* grp = IwGetResManager()->GetGroupNamed("sprites");	
			frame.image = (CIwTexture*)grp->GetResNamed(frametpl.imageresource.c_str(), "CIwTexture");
			IwAssertMsg(MYAPP, frame.image, ("Could not load texture '%s' because it seems to be missing in the resource file.", "image_missing"));
			if (!frame.image) {
				frame.image = (CIwTexture*)grp->GetResNamed("image_missing", "CIwTexture");
				IwAssertMsg(MYAPP, frame.image, ("Could not load texture '%s' because it seems to be missing in the resource file.", "image_missing"));
			}			
		} else {
			frame.colour = frametpl.colour;
		}

		m_xFrames.append(frame);
	}
}

bool Texture::SelectFirstFrame() {
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

bool Texture::SelectFrame(const std::string& id) {
	// select frame by name (max health)
	return SelectFrame(id, 100);
}

bool Texture::ReSelectFrame(int health) {
	// re-select same frame, but with new health value
	return SelectFrame(m_rxCurrentFrame.id, health);
}

bool Texture::SelectFrame(const std::string& id, int health) {
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

void Texture::SetHorizontalFlip(bool flip) {
	m_bHorizontalFlip = flip;
}

bool Texture::GetHorizontalFlip() {
	return m_bHorizontalFlip;
}

void Texture::Update(uint16 timestep) {
	// progress the animations
	if (m_rxCurrentFrame.type == eFrameTypeAnimation) {
		m_iFrameElapsed += timestep;
		if (m_iFrameElapsed >= m_rxCurrentFrame.duration) {
			m_iFrameElapsed -= m_rxCurrentFrame.duration;
			SelectFrame(m_rxCurrentFrame.nextid);
			IwDebugTraceLinePrintf("Animated: Switched to frame '%s'", m_rxCurrentFrame.nextid.c_str());
		}
	} else {
		m_iFrameElapsed = 0;
	}
}
