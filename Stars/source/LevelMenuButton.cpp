#include <sstream>


#include "LevelMenuButton.h"
#include "FactoryManager.h"
#include "UserSettings.h"

LevelMenuButton::LevelMenuButton() :
ButtonEx(eButtonCommandIdOpenLevel, s3eKeyFirst, -1),
m_iAchievedStars(0),
m_bLevelAvailable(false),
m_bReentrant(false),
m_pxStars(NULL) {
}

void LevelMenuButton::SetTextures(const std::string& facetexture, const std::string& starstexture) {
	SetTexture(FactoryManager::GetTextureFactory().Create(facetexture));
	m_pxStars = FactoryManager::GetTextureFactory().Create(starstexture);
}

void LevelMenuButton::SetLevel(int level, bool available, bool open, int stars) {
	SetUserData(level);
	m_bLevelAvailable = available;
	m_bLevelOpen = open;
	m_iAchievedStars = stars;
	
	if (open) {
		std::ostringstream oss;
		oss << level;
		SetText(oss.str(), 0xffccfaff, Renderer::eFontTypeLarge);
	} else {
		SetText("");
	}

	if (m_pxStars) {
		std::ostringstream oss;
		oss << stars;
		m_pxStars->SelectFrame(oss.str());
	}
	
	SetBehavior(IsEnabled());
}

void LevelMenuButton::OnStateChanged(bool enabled, bool pressed) {
	ButtonEx::OnStateChanged(enabled, pressed);
	SetBehavior(enabled);
}

void LevelMenuButton::OnUpdate(const FrameData& frame) {
	ButtonEx::OnUpdate(frame);
	if (m_pxStars) {
		m_pxStars->Update(frame.GetRealDurationMs());
	}
}

void LevelMenuButton::OnRender(Renderer& renderer, const FrameData& frame) {
	ButtonEx::OnRender(renderer, frame);
	if (m_pxStars && m_bLevelAvailable) {
		// calculate the size of the stars area
		CIwRect rect = GetPosition();
		rect.y += rect.h;
		rect.h /= 4;
		rect.y -= rect.h;

		// draw
		VertexStreamScreen geom;
		geom.SetRect(rect);
		renderer.Draw(geom, *m_pxStars);
	}
}

void LevelMenuButton::SetBehavior(bool enabled) {
	if (m_bReentrant) {
		// protecting against recursive SetEnabled(bool) calls
		return;
	}

	m_bReentrant = true;
	
	SetHideWhenDisabled(!m_bLevelAvailable);
	SetEnabled(enabled && m_bLevelOpen);

	m_bReentrant = false;
}
