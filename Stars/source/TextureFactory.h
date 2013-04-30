#ifndef __TEXTUREFACTORY_H__
#define __TEXTUREFACTORY_H__

#include "FactoryBase.h"
#include "TextureTemplate.h"
#include "Texture.h"

class TextureFactory : public FactoryBase<TextureTemplate, Texture> {
public:
	TextureFactory();

public:
	virtual std::string PopulateConfig(TiXmlElement* node, TextureTemplate& conf);
	virtual Texture* CreateInstance(const TextureTemplate& conf);

private:
	uint32 HexToColour(const std::string& col);
};

#endif
