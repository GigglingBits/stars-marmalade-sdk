#include "PageBackground.h"
#include "FactoryManager.h"
#include "Debug.h"

PageBackground::PageBackground() : m_pxBackground(NULL){
}

PageBackground::~PageBackground() {
	if (m_pxBackground) {
		delete m_pxBackground;
	}
}

void PageBackground::Initialize() {
	m_pxBackground = FactoryManager::GetTextureFactory().Create("background_stars");
}

void PageBackground::OnUpdate(const FrameData& frame) {
	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
    }
}

void PageBackground::OnRender(Renderer& renderer, const FrameData& frame) {
	const float MULT = 1.5f;
    
	if (m_pxBackground) {
		const LocationServices::DeviceOrientation& orientation = LocationServices::GetInstance().GetDeviceOrientation();
		CIwSVec2 orientationoffset(orientation.x * -MULT, orientation.y * MULT);

		int margin = GetScreenExtents() / 10;
		
        const CIwSVec2& screensize = frame.GetScreensize();
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

