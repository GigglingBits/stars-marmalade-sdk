#include "HudBuffButton.h"
#include "FactoryManager.h"
#include "UserSettings.h"

#include <sstream>

#include "Debug.h"
#include "IwDebug.h"

HudBuffButton::HudBuffButton(const std::string& skin, ButtonCommandId cmdid, s3eKey key) :
ButtonEx(cmdid, key, -1),
m_sSkin(skin) {
}

void HudBuffButton::SetFillDegree(float n) {
	m_fFillPercent = n * 100.0f;
	
	if (n > 0) {
		std::ostringstream oss;
		oss.precision(1);
		oss << std::fixed;
		oss << m_fFillPercent << "%";
		SetText(oss.str());
	} else {
		SetText("");
	}
}

void HudBuffButton::OnTextureLoaded(Texture& texture) {
	IW_CALLSTACK_SELF;
	
	IwAssert(MYAPP, texture.IsSkeleton());
	if (!texture.IsSkeleton()) {
		return;
	}
	
	if (SpineAnimation* skeleton = texture.GetSkeleton()) {
		skeleton->SetSkin(m_sSkin);
	}

	ButtonEx::OnTextureLoaded(texture);
}

void HudBuffButton::OnRender(Renderer& renderer, const FrameData& frame) {
	
	ButtonEx::OnRender(renderer, frame);
}
