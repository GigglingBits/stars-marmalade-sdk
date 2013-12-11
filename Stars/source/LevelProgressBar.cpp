#include "LevelProgressBar.h"
#include "FactoryManager.h"
#include "VertexStreamScreen.h"
#include "Debug.h"

LevelProgressBar::LevelProgressBar() : 
	m_pxBackground(NULL),
	m_pxStar(NULL),
	m_fProgress(0.0f) {
	IW_CALLSTACK_SELF;
	SetRederingLayer(Renderer::eRenderingLayerHud);
}

LevelProgressBar::~LevelProgressBar() {
	if (m_pxStar) {
		delete m_pxStar;
	}
	if (m_pxBackground) {
		delete m_pxBackground;
	}
	for (Icons::iterator i = m_xIcons.begin(); i != m_xIcons.end(); i++) {
		delete i->texture;
		delete i->shape;
	}
}

void LevelProgressBar::Initialize() {
	m_pxBackground = FactoryManager::GetTextureFactory().Create("progressbar_back");
	m_pxStar = FactoryManager::GetTextureFactory().Create("progressbar_star");
}

void LevelProgressBar::UpdateStarShapes() {
	UpdateStarShape(m_fProgress, m_xStarShape);
	for (Icons::iterator i = m_xIcons.begin(); i != m_xIcons.end(); i++) {
		UpdateStarShape(i->progress, *i->shape);
	}
}

void LevelProgressBar::UpdateStarShape(float progress, VertexStreamScreen& shape) {
	CIwRect rect(GetPosition());
	int size = rect.h;
	int pos = (int)(progress * (float)(rect.w - size));
	shape.SetRect(
		rect.x + pos,
		rect.y, size, size);
}

void LevelProgressBar::SetIcon(float progress, const std::string& texture) {
	IW_CALLSTACK_SELF;
	if (Texture* t = FactoryManager::GetTextureFactory().Create(texture)) {
		Icon i;
		i.progress = progress;
		i.texture = t;
		i.shape = new VertexStreamScreen();
		UpdateStarShape(progress, *i.shape);
		m_xIcons.push_back(i);
	}
}

void LevelProgressBar::SetProgress(float progress) {
	IW_CALLSTACK_SELF;
	IwAssert(MYAPP, progress >= 0.0f && progress <= 1.0f);

	m_fProgress = progress;
	UpdateStarShape(m_fProgress, m_xStarShape);
}

void LevelProgressBar::OnDoLayout(const CIwSVec2& screensize) {
	m_xBackgroundShape.SetRect(GetPosition());
	UpdateStarShapes();
}

void LevelProgressBar::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
	}
	if (m_pxStar) {
		m_pxStar->Update(frame.GetRealDurationMs());
	}
	for (Icons::iterator i = m_xIcons.begin(); i != m_xIcons.end(); i++) {
		i->texture->Update(frame.GetRealDurationMs());
	}
}

void LevelProgressBar::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (m_pxBackground) {
		renderer.Draw(m_xBackgroundShape, *m_pxBackground);
	}
	if (m_pxStar) {
		renderer.Draw(m_xStarShape, *m_pxStar);
	}
	for (Icons::iterator i = m_xIcons.begin(); i != m_xIcons.end(); i++) {
		renderer.Draw(*i->shape, *i->texture);
	}
}
