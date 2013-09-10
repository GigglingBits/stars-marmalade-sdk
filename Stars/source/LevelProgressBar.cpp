#include "LevelProgressBar.h"
#include "FactoryManager.h"
#include "VertexStreamScreen.h"
#include "Debug.h"

LevelProgressBar::LevelProgressBar() : 
	m_pxBackground(NULL),
	m_pxStar(NULL),
	m_fProgress(0.0f),
	m_xPanelShape(0, 0, 1, 1) {
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
}

void LevelProgressBar::Initialize() {
	m_pxBackground = FactoryManager::GetTextureFactory().Create("progressbar_back");
	m_pxStar = FactoryManager::GetTextureFactory().Create("progressbar_star");
}

void LevelProgressBar::UpdateStarShape() {
	int size = m_xPanelShape.h;
	int progress = (int)(m_fProgress * (float)(m_xPanelShape.w - size));
	m_xStarShape.SetRect(
		m_xPanelShape.x + progress, 
		m_xPanelShape.y, size, size);
}

void LevelProgressBar::SetPosition(const CIwRect& rect) {
	m_xPanelShape = rect;
	InvalidateLayout();
}

void LevelProgressBar::SetProgress(float progress) {
	IW_CALLSTACK_SELF;
	IwAssert(MYAPP, progress >= 0.0f && progress <= 1.0f);

	m_fProgress = progress;

	UpdateStarShape();
}

void LevelProgressBar::OnDoLayout(const CIwSVec2& screensize) {
	m_xBackgroundShape.SetRect(m_xPanelShape);	
	UpdateStarShape();
}

void LevelProgressBar::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
	}

	if (m_pxStar) {
		m_pxStar->Update(frame.GetRealDurationMs());
	}
}

void LevelProgressBar::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	// background
	if (m_pxBackground) {
		renderer.Draw(m_xBackgroundShape, *m_pxBackground);
	}
	if (m_pxStar) {
		renderer.Draw(m_xStarShape, *m_pxStar);
	}
}
