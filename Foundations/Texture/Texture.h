#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "IwList.h"
#include "IwTexture.h"
#include "BufferedAnimTexture.h"
#include "TextureTemplate.h"

class Texture {
public:
	virtual ~Texture();
	
	// capabilities
public:
	virtual bool IsImage();
	virtual CIwTexture* GetImage();

	virtual bool IsPattern();
	virtual CIwTexture* GetPattern();

	virtual bool IsColour();
	virtual uint32 GetColour();
	
	virtual bool IsSkeleton();
	virtual BufferedAnimTexture* GetSkeleton();
	
	// behaviors
public:
	virtual bool ContainsFrame(const std::string name) = 0;
	
	virtual bool SelectFrame(const std::string& id) = 0;
	virtual bool SelectFrame(const std::string& id, int health) = 0;
	virtual bool ReSelectFrame(int health) = 0;

	virtual void SetHorizontalFlip(bool flip) = 0;
	virtual bool GetHorizontalFlip() = 0;

	virtual void Update(uint16 timestep) = 0;
	
public:
	static Texture* Create(const TextureTemplate& tpl);
};

#endif
