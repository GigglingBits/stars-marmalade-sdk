#include "Texture.h"
#include "TextureAnimated.h"
#include "TextureFramed.h"
#include "Texture.h"

Texture::~Texture() {
	// needed for invoking destructors of inheriting classes
}

bool Texture::IsImage() {
	return false;
}

CIwTexture* Texture::GetImage() {
	return NULL;
}

bool Texture::IsSkeleton() {
	return false;
}

BufferedAnimTexture* Texture::GetSkeleton() {
	return NULL;
}

bool Texture::IsPattern() {
	return false;
}

CIwTexture* Texture::GetPattern() {
	return NULL;
}

bool Texture::IsColour() {
	return false;
}

uint32 Texture::GetColour() {
	return 0x00000000;
}

Texture* Texture::Create(const TextureTemplate& tpl) {
	if (tpl.IsSkeletonAnimation()) {
		return new TextureAnimated(tpl);
	} else {
		return new TextureFramed(tpl);
	}
}
