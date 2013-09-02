#include "Renderable.h"

Renderable::Renderable() {
	m_eRenderingLayer = Renderer::eRenderingLayerGameDynamicObjects;
}

Renderable::~Renderable() {
}

void Renderable::SetDefaultRederingLayer() {
	SetRederingLayer(Renderer::eRenderingLayerGameDynamicObjects);
}

void Renderable::SetRederingLayer(Renderer::RenderingLayer renderinglayer) {
	m_eRenderingLayer = renderinglayer;
}

void Renderable::Update(const FrameData& frame) {
	OnUpdate(frame);
}

void Renderable::Render(Renderer& renderer, const FrameData& frame) {
	renderer.SetRederingLayer(m_eRenderingLayer);
	OnRender(renderer, frame);
	renderer.SetDefaultRederingLayer();
}
