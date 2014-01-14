#include "Renderable.h"

Renderable::Renderable() {
	m_eRenderingLayer = Renderer::eRenderingLayerGameDynamicObjects;
}

Renderable::~Renderable() {
}

void Renderable::SetDefaultRederingLayer() {
	SetRenderingLayer(Renderer::eRenderingLayerGameDynamicObjects);
}

void Renderable::SetRenderingLayer(Renderer::RenderingLayer renderinglayer) {
	m_eRenderingLayer = renderinglayer;
}

Renderer::RenderingLayer Renderable::GetRederingLayer() {
	return m_eRenderingLayer;
}

void Renderable::Update(const FrameData& frame) {
	OnUpdate(frame);
}

void Renderable::Render(Renderer& renderer, const FrameData& frame) {
	renderer.SetRenderingLayer(m_eRenderingLayer);
	OnRender(renderer, frame);
	renderer.SetDefaultRederingLayer();
}
