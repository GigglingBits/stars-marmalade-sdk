#include "LevelDustVial.h"
#include "FactoryManager.h"
#include "VertexStreamScreen.h"
#include "Debug.h"

uint32 LevelDustVial::s_auCommittedColours[] = {
	0xdd37f5eb,
	0xdd37f5eb,
	0xdd44cde6,
	0xdd44cde6
};

uint32 LevelDustVial::s_auQueuedColours[] = {
	0xddccf5eb,
	0xddccf5eb,
	0xddeff5eb,
	0xddeff5eb
};

LevelDustVial::LevelDustVial() :
m_fCommittedDust(0.0f),
m_fQueuedDust(0.0f),
m_pxBack(NULL),
m_pxCommittedDustSurface(NULL),
m_pxQueuedDustSurface(NULL) {
	SetRenderingLayer(Renderer::eRenderingLayerHud);
}

LevelDustVial::~LevelDustVial() {
	if (m_pxBack) {
		delete m_pxBack;
	}
	if (m_pxCommittedDustSurface) {
		delete m_pxCommittedDustSurface;
	}
	if (m_pxQueuedDustSurface) {
		delete m_pxQueuedDustSurface;
	}
}

void LevelDustVial::Initialize() {
	// we misuse this to draw the foreground
	SetBackground(FactoryManager::GetTextureFactory().Create("stardustvial"));
	m_pxBack = FactoryManager::GetTextureFactory().Create("stardustvial_back");
	m_pxCommittedDustSurface = FactoryManager::GetTextureFactory().Create("stardustsurface");
	m_pxQueuedDustSurface = FactoryManager::GetTextureFactory().Create("stardustsurface_queued");
}

void LevelDustVial::SetDustAmount(float committed, float queued) {
	committed = std::min<float>(committed, 1.0f);
	committed = std::max<float>(committed, 0.0f);
	queued = std::min<float>(queued, 1.0f - m_fCommittedDust);
	queued = std::max<float>(queued, 0.0f);
	
	if (committed != m_fCommittedDust || queued != m_fQueuedDust) {
		m_fCommittedDust = committed;
		m_fQueuedDust = queued;
		UpdateAmountShapes();
	}
}

void LevelDustVial::UpdateVialShapes() {
	// vial background geometry
	CIwRect back = GetPosition();
	int marginx = std::max<int>(1, back.w / 50);
	int marginy = std::max<int>(1, back.h / 5);
	back.x += marginx;
	back.y += marginy;
	back.w -= 2 * marginx;
	back.h -= 2 * marginy;
	m_xBackShape.SetRect(back);
	m_xBackShape.ClosePolygon();
	
	// vial content geometry
	m_xContentGeom = back;
	marginx = std::max<int>(1, m_xContentGeom.w / 12);
	marginy = std::max<int>(1, m_xContentGeom.h / 5);
	m_xContentGeom.x += marginx;
	m_xContentGeom.y += marginy;
	m_xContentGeom.w -= 2 * marginx;
	m_xContentGeom.h -= marginy;
}

void LevelDustVial::UpdateAmountShapes() {
	int surfaceheight = m_xContentGeom.w / 3;
	int surfaceorigin = surfaceheight * 10 / 11;

	CIwRect committed = m_xContentGeom;
	committed.h = m_xContentGeom.h * m_fCommittedDust;
	committed.y += m_xContentGeom.h - committed.h;
	m_xCommittedDustShape.SetRect(committed);
	
	m_xCommittedDustSurfaceShape.SetRect(
		committed.x, committed.y - surfaceorigin,
		committed.w, surfaceheight);

	CIwRect queued = m_xContentGeom;
	queued.h = m_xContentGeom.h * m_fQueuedDust;
	queued.y += m_xContentGeom.h - queued.h - committed.h;
	m_xQueuedDustShape.SetRect(queued);
	
	m_xQueuedDustSurfaceShape.SetRect(
		queued.x, queued.y - surfaceorigin,
		queued.w, surfaceheight);
}

void LevelDustVial::OnDoLayout(const CIwSVec2& screensize) {
	UpdateVialShapes();
	UpdateAmountShapes();
}

void LevelDustVial::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;
	
	if (m_pxBack) {
		m_pxBack->Update(frame.GetRealDurationMs());
	}
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
	renderer.SetRenderingLayer(Renderer::eRenderingLayerHud3);
	if (m_pxBack) {
		renderer.Draw(m_xBackShape, *m_pxBack);
	}
	
	// vial content
	renderer.SetRenderingLayer(Renderer::eRenderingLayerHud2);
	if (m_fCommittedDust > 0.0f) {
		renderer.DrawPolygon(
			m_xCommittedDustShape.GetVerts(), m_xCommittedDustShape.GetVertCount(),
			NULL, s_auCommittedColours);
		if (m_fQueuedDust <= 0.0f && m_pxCommittedDustSurface) {
			renderer.Draw(m_xCommittedDustSurfaceShape, *m_pxCommittedDustSurface);
		}
	}
	if (m_fQueuedDust > 0.0f) {
		renderer.DrawPolygon(
			m_xQueuedDustShape.GetVerts(), m_xQueuedDustShape.GetVertCount(),
			NULL, s_auQueuedColours);
		if (m_pxQueuedDustSurface) {
			renderer.Draw(m_xQueuedDustSurfaceShape, *m_pxQueuedDustSurface);
		}
	}
}
