#ifndef __REDNERABLE_H__
#define __REDNERABLE_H__

#include "IwGeom.h"
#include "Renderer.h"
#include "FrameData.h"

class Renderable {
private:
	Renderer::RenderingLayer m_eRenderingLayer;

public:
	Renderable();
	virtual ~Renderable();

	void SetDefaultRederingLayer();
	void SetRenderingLayer(Renderer::RenderingLayer renderinglayer);
	
	virtual void Update(const FrameData& frame);
	virtual void Render(Renderer& renderer, const FrameData& frame);

protected:
	Renderer::RenderingLayer GetRederingLayer();
	
	virtual void OnUpdate(const FrameData& frame) = 0;
	virtual void OnRender(Renderer& renderer, const FrameData& frame) = 0;
};

#endif
