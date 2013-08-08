#include "LevelStatsPanel.h"
#include "FactoryManager.h"
#include "VertexStreamScreen.h"
#include "Debug.h"

LevelStatsPanel::LevelStatsPanel() : 
	m_pxBackground(NULL),
	m_pxStar(NULL),
	m_fProgress(0.0f) {
	IW_CALLSTACK_SELF;
	SetRederingLayer(Renderer::eRenderingLayerHud);
}

LevelStatsPanel::~LevelStatsPanel() {
	if (m_pxStar) {
		delete m_pxStar;
	}
	if (m_pxBackground) {
		delete m_pxBackground;
	}
}

void LevelStatsPanel::Initialize() {
	m_pxBackground = FactoryManager::GetTextureFactory().Create("statspanel");
	m_pxStar = FactoryManager::GetTextureFactory().Create("star");
}

void LevelStatsPanel::UpdateStarShape() {
	int size = m_xPanelShape.h;
	int progress = (int)(m_fProgress * (float)(m_xPanelShape.w - size));
	m_xStarShape.SetRect(
		m_xPanelShape.x + progress, 
		m_xPanelShape.y, size, size);
}

void LevelStatsPanel::SetProgress(float progress) {
	IW_CALLSTACK_SELF;
	IwAssert(MYAPP, progress >= 0.0f && progress <= 1.0f);

	m_fProgress = progress;

	UpdateStarShape();
}

void LevelStatsPanel::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
	}

	if (m_pxStar) {
		m_pxStar->Update(frame.GetRealDurationMs());
	}
}

void LevelStatsPanel::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	// background
	if (m_pxBackground) {
		renderer.Draw(m_xBackgroundShape, *m_pxBackground);
	}
	if (m_pxStar) {
		renderer.Draw(m_xStarShape, *m_pxStar);
	}
}

void LevelStatsPanel::OnDoLayout(const CIwSVec2& screensize) {
	const int h = 40, w = screensize.x / 2;
	int x = (screensize.x - w) / 2, y = h / 2;

	m_xPanelShape.Make(x, y, w, h);
	m_xBackgroundShape.SetRect(m_xPanelShape);

	UpdateStarShape();
}
