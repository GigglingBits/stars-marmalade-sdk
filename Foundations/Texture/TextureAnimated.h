#ifndef __TEXTUREANIMATED_H__
#define __TEXTUREANIMATED_H__

#include "Texture.h"
#include "BufferedAnimTexture.h"
#include "TextureTemplate.h"

class TextureAnimated : public Texture {
private:
	BufferedAnimTexture m_xAnimation;
	
public:
	TextureAnimated(const TextureTemplate& texturedef);

	// capabilities
public:
	virtual bool IsSkeleton();
	virtual BufferedAnimTexture* GetSkeleton();
	
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
	
private:
	void Load(const std::string& skeleton);
};

#endif
