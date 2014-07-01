#include "HudBuffPanel.h"
#include "FactoryManager.h"

HudBuffPanel::HudBuffPanel() :
m_bIsEnabled(true) {
}

HudBuffPanel::~HudBuffPanel() {
	ClearBuffs();
}


void HudBuffPanel::Initialize() {
}

void HudBuffPanel::SetEnabled(bool enabled) {
	if (m_bIsEnabled != enabled) {
		std::vector<HudBuffButton*>::const_iterator it;
		for (it = m_xBuffs.begin(); it != m_xBuffs.end(); ++it) {
			if (*it) {
				(*it)->SetEnabled(enabled);
			}
		}
	}
	
	m_bIsEnabled = enabled;
}

void HudBuffPanel::AddBuff(GameFoundation::BuffType bt) {
	HudBuffButton* btn = new HudBuffButton(bt);
	btn->BuffTrigger.AddListener(this, &HudBuffPanel::BuffTriggerHandler);
	m_xBuffs.push_back(btn);
	
	InvalidateLayout();
}

void HudBuffPanel::ClearBuffs() {
	std::vector<HudBuffButton*>::const_iterator it;
	for (it = m_xBuffs.begin(); it != m_xBuffs.end(); ++it) {
		if (*it) {
			(*it)->BuffTrigger.RemoveListener(this, &HudBuffPanel::BuffTriggerHandler);
			delete *it;
		}
	}
	m_xBuffs.clear();
}

void HudBuffPanel::OnDoLayout(const CIwSVec2& screensize) {
	int extent = GetScreenExtents();
	
	int spacing = extent / 40;
	int buttonwidth = extent / 8;
	int buttonheight = extent / 8;
	
	// action buttons (right)
	CIwRect rect;
	rect.x = spacing;
	rect.y = screensize.y - (buttonheight + spacing);
	rect.w = buttonwidth;
	rect.h = buttonheight;

	std::vector<HudBuffButton*>::const_iterator it;
	for (it = m_xBuffs.begin(); it != m_xBuffs.end(); ++it) {
		if (*it) {
			(*it)->SetPosition(rect);
			rect.y -= rect.h + spacing;
		}
	}
}

void HudBuffPanel::OnUpdate(const FrameData& frame) {
	std::vector<HudBuffButton*>::iterator it = m_xBuffs.begin();
	while (it != m_xBuffs.end()) {
		if (!*it || (*it)->CanUnload()) {
			(*it)->BuffTrigger.RemoveListener(this, &HudBuffPanel::BuffTriggerHandler);
			delete *it;
			it = m_xBuffs.erase(it);
			InvalidateLayout();
		} else {
			(*it)->Update(frame);
			++it;
		}
	}
}

void HudBuffPanel::OnRender(Renderer& renderer, const FrameData& frame) {
	if (!m_bIsEnabled) {
		return;
	}
	
	std::vector<HudBuffButton*>::const_iterator it;
	for (it = m_xBuffs.begin(); it != m_xBuffs.end(); ++it) {
		if (*it) {
			(*it)->Render(renderer, frame);
		}
	}
}

void HudBuffPanel::BuffTriggerHandler(const HudBuffButton& sender, const GameFoundation::BuffType& bt) {
	BuffTrigger.Invoke(*this, bt);
}
