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
    if (m_pxBackground) {
		const s3eGyroscopeData& gyrodata = LocationServices::GetInstance().GetGyroData();
		CIwSVec2 gyrooffset(gyrodata.m_X * 5.0f, gyrodata.m_Y * 5.0f);
		
        VertexStreamScreen shape;
        const CIwSVec2& screensize = frame.GetScreensize();
        shape.SetRect(CIwRect(gyrooffset.x, gyrooffset.y, screensize.x, screensize.y));
        renderer.Draw(shape, *m_pxBackground);
    }	
}

