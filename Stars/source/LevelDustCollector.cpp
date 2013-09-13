#include "LevelDustCollector.h"
#include "FactoryManager.h"
#include "VertexStreamScreen.h"
#include "Debug.h"

LevelDustCollector::LevelDustCollector() : 
	m_pxVial(NULL),
	m_pxDust(NULL),
	m_fTargetProgress(0.0f),
	m_fDisplayedProgress(0.0f),
	m_xCollectorShape(0, 0, 1, 1) {
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

void LevelDustCollector::SetPosition(const CIwRect& rect) {
	m_xCollectorShape = rect;
	InvalidateLayout();
}

void LevelDustCollector::UpdateDustShape() {
	int progress = (int)(m_fDisplayedProgress * (float)(m_xCollectorShape.h));
	m_xDustShape.SetRect(
		m_xCollectorShape.x,
		m_xCollectorShape.y + (m_xCollectorShape.h - progress),
		m_xCollectorShape.w,
		progress);
	m_xDustShape.ClosePolygon();
}

void LevelDustCollector::SetProgress(float progress, int rolltime) {
	IW_CALLSTACK_SELF;
	progress = std::min<float>(progress, 1.0f);

	if (m_fTargetProgress == progress) {
		return;
	}

	m_fTargetProgress = progress;
	m_lTotalRollTime = rolltime;
	m_lRemainingRollTime = m_lTotalRollTime;
	
	UpdateDustShape();
}

void LevelDustCollector::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	// needs rolling?
	if (m_fDisplayedProgress != m_fTargetProgress) {
		// time exceeded?
		if (m_lRemainingRollTime <= 0) {
			m_fDisplayedProgress = m_fTargetProgress;
		} else {
			// roll the number
			float error = m_fTargetProgress - m_fDisplayedProgress;
			float correction = error * (m_lTotalRollTime - m_lRemainingRollTime) / m_lTotalRollTime;
			m_fDisplayedProgress += correction;
		}
		
		m_lRemainingRollTime -= frame.GetSimulatedDurationMs();
		UpdateDustShape();
	}
	
	// textures
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
	m_xVialShape.SetRect(m_xCollectorShape);
	UpdateDustShape();
}
