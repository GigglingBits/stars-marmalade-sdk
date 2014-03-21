#include "SplashText.h"
#include "Debug.h" 

SplashText::SplashText(const std::string& id, const ShapeTemplate& shapedef, const TextureTemplate& texturedef) 
	: LocalEffect(id, shapedef, texturedef), m_uiColour(0xffffffff) {
}

const char* SplashText::GetTypeName() {
	return SplashText::TypeName();
}

const char* SplashText::TypeName() {
	static const char* type = "splashtext";
	return type;
}

void SplashText::SetText(std::string text, uint32 colour) {
	m_sText = text;
	m_uiColour = colour;
}

void SplashText::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;
	LocalEffect::OnRender(renderer, frame);

	if (m_sText.empty()) {
		return;
	}
	renderer.DrawText(m_sText, GetPosition(), Renderer::eFontTypeNormal, m_uiColour);
}
