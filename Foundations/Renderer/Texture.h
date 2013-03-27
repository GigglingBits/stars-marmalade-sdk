#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "IwList.h"
#include "IwTexture.h"
#include "TextureTemplate.h"

class Texture {
	class InternalFrame : public TextureFrame {
	public:
		CIwTexture* image;
		InternalFrame() : image(NULL) {}
	};

private:
	bool m_bHorizontalFlip;

	uint32 m_iFrameElapsed;

	typedef CIwList<InternalFrame> FrameList;
	FrameList m_xFrames;

	InternalFrame& m_rxCurrentFrame;
    InternalFrame m_xNullFrame;

public:
	Texture(const TextureTemplate& texturedef);

	bool IsImage();
	CIwTexture* GetImage();

	bool IsPattern();
	CIwTexture* GetPattern();

	bool IsColour();
	uint32 GetColour();

	bool SelectFirstFrame();
	bool SelectFrame(const std::string& id);
	bool SelectFrame(const std::string& id, int health);
	bool ReSelectFrame(int health);

	void SetHorizontalFlip(bool flip);
	bool GetHorizontalFlip();

	void Update(uint16 timestep);

protected:
	void LoadFrames(TextureTemplate& texturedef);
};

#endif
