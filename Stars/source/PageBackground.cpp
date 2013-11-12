#include "PageBackground.h"
#include "FactoryManager.h"
#include "Debug.h"
#include "DeviceInfo.h"

PageBackground::PageBackground() : m_pxBackground(NULL), m_fParallaxCorrection(0.0f) {
}

PageBackground::~PageBackground() {
	if (m_pxBackground) {
		delete m_pxBackground;
	}
}

void PageBackground::Initialize() {
	m_pxBackground = FactoryManager::GetTextureFactory().Create("background_stars");

	m_fParallaxCorrection = DeviceInfo::GetInstance().GetPixelsPerInch() * 0.003f;
}

void PageBackground::OnUpdate(const FrameData& frame) {
	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
    }
}

void PageBackground::OnRender(Renderer& renderer, const FrameData& frame) {
	if (m_pxBackground) {
        const CIwSVec2& screensize = frame.GetScreensize();
		int margin = GetScreenExtents() / 10;

		const LocationServices::DeviceOrientation& orientation =
			LocationServices::GetInstance().GetDeviceOrientation();

		CIwSVec2 orientationoffset(
			orientation.x * -m_fParallaxCorrection,
			orientation.y * m_fParallaxCorrection);

		CIwRect rect(
			orientationoffset.x - margin,
			orientationoffset.y - margin,
			screensize.x + (2 * margin),
			screensize.y + (2 * margin));
		
        VertexStreamScreen shape;
        shape.SetRect(rect);

        renderer.Draw(shape, *m_pxBackground);
    }	
}

