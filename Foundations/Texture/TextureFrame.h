#ifndef __TEXTUREFRAME_H__
#define __TEXTUREFRAME_H__

#include <string>
#include "IwTexture.h"

enum eFrameType {
	eFrameTypeImage,
	eFrameTypeColour,
	eFrameTypePattern,
	eFrameTypeAnimation
};

class TextureFrame {
public:
	eFrameType type;
	std::string id;
	int healthlevel;
	std::string imageresource;
	uint32 colour;
	uint duration;
	std::string nextid;

public:
	TextureFrame();
};

#endif
