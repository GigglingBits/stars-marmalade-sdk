#include "SplashText.h"
#include "Debug.h" 

SplashText::SplashText(const std::string& id, const ShapeTemplate& shapedef, const TextureTemplate& texturedef) 
	: LocalEffect(id, shapedef, texturedef) {
}

const char* SplashText::GetTypeName() {
	return SplashText::TypeName();
}

const char* SplashText::TypeName() {
	static const char* type = "splashtext";
	return type;
}

void SplashText::SetText(std::string text) {
	m_sText = text;
}

void SplashText::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;
	LocalEffect::OnRender(renderer, frame);

	if (m_sText.empty()) {
		return;
	}
	renderer.DrawText(m_sText, GetPosition(), Renderer::eFontTypeSmall, 0xff00ffff);
}
