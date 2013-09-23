#ifndef __TEXTUREVIEW_H__
#define __TEXTUREVIEW_H__

#include <string>
#include "Texture.h"
#include "MediaView.h"
#include "VertexStreamScreen.h"

class TextureView : public MediaView {
private:
	Texture* m_pxTexture;
	VertexStreamScreen m_xGemoetry;
	
public:
	TextureView(const std::string textureid);
	virtual ~TextureView();

	virtual void Initialize();

protected:
	virtual void OnDoLayout(const CIwSVec2& screensize);
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
