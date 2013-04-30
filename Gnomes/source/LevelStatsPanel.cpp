#include "LevelStatsPanel.h"
#include "FactoryManager.h"
#include "VertexStreamScreen.h"
#include "Debug.h"

LevelStatsPanel::LevelStatsPanel() : 
	m_pxBackground(NULL),
	m_xNullButton(eButtonCommandIdSetHeroFocus, s3eKeyFirst), 
	m_rxSelected(m_xNullButton) {
	IW_CALLSTACK_SELF;
	SetRederingLayer(Renderer::eRenderingLayerHud);
}

LevelStatsPanel::~LevelStatsPanel() {
	for (HeroMap::iterator it = m_xHeroMap.begin(); it != m_xHeroMap.end(); it++) {
		Button* btn = it->second;
		it->second = NULL;

		btn->PressedEvent.RemoveListener(this, &LevelStatsPanel::ButtonPressedEventHandler);
		delete btn;
	}

	if (m_pxBackground) {
		delete m_pxBackground;
	}
}

void LevelStatsPanel::Initialize() {
	m_pxBackground = FactoryManager::GetTextureFactory().Create("statspanel");
}

void LevelStatsPanel::SetEnabled(bool enabled) {
	for (HeroMap::iterator it = m_xHeroMap.begin(); it != m_xHeroMap.end(); it++) {
		it->second->SetEnabled(enabled);
	}
}

void LevelStatsPanel::Select(const std::string& id) {
	IW_CALLSTACK_SELF;

	HeroMap::iterator it = m_xHeroMap.find(id);
	IwAssert(MYAPP, it != m_xHeroMap.end());
	Button* selected = it->second;
	m_rxSelected = selected ? *selected : m_rxSelected;
}

void LevelStatsPanel::Kill(const std::string& id) {
	IW_CALLSTACK_SELF;
	HeroMap::iterator it = m_xHeroMap.find(id);
	IwAssert(MYAPP, it != m_xHeroMap.end());
	it->second->SetTextureFrame("killed");
	it->second->SetEnabled(false);
}

void LevelStatsPanel::Rescue(const std::string& id) {
	IW_CALLSTACK_SELF;
	HeroMap::iterator it = m_xHeroMap.find(id);
	IwAssert(MYAPP, it != m_xHeroMap.end());
	it->second->SetTextureFrame("rescued");
	it->second->SetEnabled(false);
}

void LevelStatsPanel::Add(const std::string& id) {
	IW_CALLSTACK_SELF;
	IwAssert(MYAPP, m_xHeroMap.find(id) == m_xHeroMap.end());
	Button* btn = new Button(eButtonCommandIdSetHeroFocus, s3eKeyFirst);
	btn->SetTexture(FactoryManager::GetTextureFactory().Create("hero_stats"));
	btn->SetTextureFrame("faded");
	btn->SetHideWhenDisabled(false);
	btn->SetRederingLayer(Renderer::eRenderingLayerHud);
	btn->PressedEvent.AddListener(this, &LevelStatsPanel::ButtonPressedEventHandler);
	m_xHeroMap[id] = btn;
}

void LevelStatsPanel::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
	}

	for (HeroMap::iterator it = m_xHeroMap.begin(); it != m_xHeroMap.end(); it++) {
		it->second->Update(frame);
	}
}

void LevelStatsPanel::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	// background
	if (m_pxBackground) {
		renderer.Draw(m_xBackgroundShape, *m_pxBackground);
	}

	// buttons
	for (HeroMap::iterator it = m_xHeroMap.begin(); it != m_xHeroMap.end(); it++) {
		Button* btn = it->second;
		if (btn == &m_rxSelected) {
			
		}

		it->second->Render(renderer, frame);
	}
}

void LevelStatsPanel::OnDoLayout(const CIwSVec2& screensize) {
	const int h = 40, w = 240;
	int x = (screensize.x - w) / 2, y = screensize.y - h;

	// background
	m_xBackgroundShape.SetRect(x, y, w, h);

	// buttons
	int i = 0;
	for (HeroMap::iterator it = m_xHeroMap.begin(); it != m_xHeroMap.end(); it++) {
		it->second->SetPosition(CIwRect(x + (i++) * h, y, h, h));
	}
}

void LevelStatsPanel::ButtonPressedEventHandler(const Button& sender, const Button::EventArgs& args) {
	for (HeroMap::iterator it = m_xHeroMap.begin(); it != m_xHeroMap.end(); it++) {
		if (it->second == &sender) {
			HeroFocusRequested.Invoke(*this, it->first);
			return;
		}
	}
	IwAssertMsg(MYAPP, false, ("Hero focus change requested, but hero id cannot be found."));
}
