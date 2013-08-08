#include "LevelDustCollector.h"
#include "FactoryManager.h"
#include "VertexStreamScreen.h"
#include "Debug.h"

LevelDustCollector::LevelDustCollector() : 
	m_pxVial(NULL),
	m_pxDust(NULL),
	m_fProgress(0.0f) {
	IW_CALLSTACK_SELF;
	SetRederingLayer(Renderer::eRenderingLayerHud);
}

LevelDustCollector::~LevelDustCollector() {
	if (m_pxDust) {
		delete m_pxDust;
	}
	if (m_pxVial) {
		delete m_pxVial;
	}
}

void LevelDustCollector::Initialize() {
	m_pxVial = FactoryManager::GetTextureFactory().Create("stardustvial");
	m_pxDust = FactoryManager::GetTextureFactory().Create("stardust");
}

void LevelDustCollector::UpdateStarShape() {
	int progress = (int)(m_fProgress * (float)(m_xCollectorShape.h));
	m_xDustShape.SetRect(
		m_xCollectorShape.x,
		m_xCollectorShape.y + (m_xCollectorShape.h - progress),
		m_xCollectorShape.w,
		progress);
	m_xDustShape.ClosePolygon();
}

void LevelDustCollector::SetProgress(float progress) {
	IW_CALLSTACK_SELF;
	IwAssert(MYAPP, progress >= 0.0f && progress <= 1.0f);

	m_fProgress = progress;

	UpdateStarShape();
}

void LevelDustCollector::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (m_pxVial) {
		m_pxVial->Update(frame.GetRealDurationMs());
	}

	if (m_pxDust) {
		m_pxDust->Update(frame.GetRealDurationMs());
	}
}

void LevelDustCollector::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	// background
	if (m_pxVial) {
		renderer.SetRederingLayer(Renderer::eRenderingLayerHud2);
		renderer.Draw(m_xVialShape, *m_pxVial);
	}
	if (m_pxDust) {
		renderer.SetRederingLayer(Renderer::eRenderingLayerHud3);
		renderer.Draw(m_xDustShape, *m_pxDust);
	}
}

void LevelDustCollector::OnDoLayout(const CIwSVec2& screensize) {
	const int w = 40, h = screensize.y - w - w;
	int x = w / 2, y = w;

	m_xCollectorShape.Make(x, y, w, h);
	m_xVialShape.SetRect(m_xCollectorShape);

	UpdateStarShape();
}
