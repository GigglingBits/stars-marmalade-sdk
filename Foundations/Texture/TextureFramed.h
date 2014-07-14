#ifndef __TEXTUREFRAMED_H__
#define __TEXTUREFRAMED_H__

#include <string>
#include "IwList.h"
#include "IwTexture.h"
#include "Texture.h"
#include "TextureTemplate.h"

class TextureFramed : public Texture {
private:
	class InternalFrame : public TextureFrame {
	public:
		eFrameType type;
		std::string id;
		int healthlevel;
		std::string imageresource;
		std::string skeletonanimation;
		uint32 colour;
		uint duration;
		std::string nextid;

	public:
		CIwTexture* image;
		InternalFrame() : image(NULL)  {}
	};
	
private:
	bool m_bHorizontalFlip;
	
	uint32 m_iFrameElapsed;
	
	typedef CIwList<InternalFrame> FrameList;
	FrameList m_xFrames;
	
	InternalFrame& m_rxCurrentFrame;
    InternalFrame m_xNullFrame;
	
public:
	TextureFramed(const TextureTemplate& texturedef);
		
	// capabilities
public:
	virtual bool IsImage();
	virtual CIwTexture* GetImage();
	
	virtual bool IsPattern();
	virtual CIwTexture* GetPattern();
	
	virtual bool IsColour();
	virtual uint32 GetColour();
	
	// behaviors
public:
	virtual bool ContainsFrame(const std::string name);
	
	virtual bool SelectFrame(const std::string& id);
	virtual bool SelectFrame(const std::string& id, int health);
	virtual bool ReSelectFrame(int health);
	
	virtual std::string GetCurrentFrame();

	virtual void SetHorizontalFlip(bool flip);
	virtual bool GetHorizontalFlip();
	
	// infrastructure
public:
	virtual void Update(uint16 timestep);

protected:
	void LoadFrames(TextureTemplate& texturedef);
	bool SelectFirstFrame();
};

#endif
