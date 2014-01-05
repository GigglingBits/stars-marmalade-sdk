#include "LevelDustVial.h"
#include "FactoryManager.h"
#include "VertexStreamScreen.h"
#include "Debug.h"

LevelDustVial::LevelDustVial() :
m_fCommittedDust(0.0f),
m_fQueuedDust(0.0f),
m_pxCommittedDustSurface(NULL),
m_pxQueuedDustSurface(NULL) {
	SetRederingLayer(Renderer::eRenderingLayerHud);
}

LevelDustVial::~LevelDustVial() {
}

void LevelDustVial::Initialize() {
	// we misuse this to draw the foreground
	SetBackground(FactoryManager::GetTextureFactory().Create("stardustvial"));

	m_pxCommittedDustSurface = FactoryManager::GetTextureFactory().Create("stardustsurface");
	m_pxQueuedDustSurface = FactoryManager::GetTextureFactory().Create("stardustsurface_queued");
}

void LevelDustVial::SetDustAmount(float committed, float queued) {
	m_fCommittedDust = std::min<float>(committed, 1.0f);
	m_fCommittedDust = std::max<float>(m_fCommittedDust, 0.0f);
	m_fQueuedDust = std::min<float>(queued, 1.0f - m_fCommittedDust);
	m_fQueuedDust = std::max<float>(m_fQueuedDust, 0.0f);
	
	UpdateShapes();
}

void LevelDustVial::UpdateShapes() {
	m_xCommittedGeom = m_xContentGeom;
	m_xCommittedGeom.h = m_xContentGeom.h * m_fCommittedDust;
	m_xCommittedGeom.y += m_xContentGeom.h - m_xCommittedGeom.h;
	
	m_xQueuedGeom = m_xContentGeom;
	m_xQueuedGeom.h = m_xContentGeom.h * m_fQueuedDust;
	m_xQueuedGeom.y += m_xContentGeom.h - m_xQueuedGeom.h - m_xCommittedGeom.h;
}

void LevelDustVial::OnDoLayout(const CIwSVec2& screensize) {
	// vial background geometry
	m_xBackGeom = GetPosition();
	int marginx = std::max<int>(1, m_xBackGeom.w / 50);
	int marginy = std::max<int>(1, m_xBackGeom.h / 5);
	m_xBackGeom.x += marginx;
	m_xBackGeom.y += marginy;
	m_xBackGeom.w -= 2 * marginx;
	m_xBackGeom.h -= 2 * marginy;
	
	// vial content geometry
	m_xContentGeom = m_xBackGeom;
	marginx = std::max<int>(1, m_xContentGeom.w / 20);
	marginy = std::max<int>(1, m_xContentGeom.h / 7);
	m_xContentGeom.x += marginx;
	m_xContentGeom.y += marginy;
	m_xContentGeom.w -= 2 * marginx;
	m_xContentGeom.h -= marginy;
	
	UpdateShapes();
}

void LevelDustVial::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;
	
	if (m_pxCommittedDustSurface) {
		m_pxCommittedDustSurface->Update(frame.GetRealDurationMs());
	}
	if (m_pxQueuedDustSurface) {
		m_pxQueuedDustSurface->Update(frame.GetRealDurationMs());
	}
}

void LevelDustVial::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;
	
	// vial background
	renderer.SetRederingLayer(Renderer::eRenderingLayerHud3);
	renderer.DrawRect(m_xBackGeom, 0x00000000, 0xcc31353a);
	
	// vial content
	renderer.SetRederingLayer(Renderer::eRenderingLayerHud2);
	int surfaceheight = m_xCommittedGeom.w / 3;
	int surfaceorigin = surfaceheight * 10 / 11;
	VertexStreamScreen shape;
	if (m_xCommittedGeom.h > 0) {
		renderer.DrawRect(m_xCommittedGeom, 0x00000000, 0xee10e3f9);
		if (m_xQueuedGeom.h <= 0 && m_pxCommittedDustSurface) {
			shape.SetRect(
				m_xCommittedGeom.x, m_xCommittedGeom.y - surfaceorigin,
				m_xCommittedGeom.w, surfaceheight);
			renderer.Draw(shape, *m_pxCommittedDustSurface);
		}
	}
	if (m_xQueuedGeom.h > 0) {
		renderer.DrawRect(m_xQueuedGeom, 0xffffffff, 0xeeffffff);
		if (m_pxQueuedDustSurface) {
			shape.SetRect(
				m_xQueuedGeom.x, m_xQueuedGeom.y - surfaceorigin,
				m_xQueuedGeom.w, surfaceheight);
			renderer.Draw(shape, *m_pxQueuedDustSurface);
		}
	}
}
