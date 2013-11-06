#include "PageBackground.h"
#include "FactoryManager.h"
#include "Debug.h"

PageBackground::PageBackground() : m_pxBackground(NULL){
	if (s3eGyroscopeAvailable()) {
		s3eGyroscopeStart();
		s3eGyroscopeSetSensorDelay(DELAY_UI);
		s3eGyroscopeRegister(S3E_GYROSCOPE_CALLBACK_DATA_UPDATE, (s3eCallback)GyroscopeCallback, this);
	}
}

PageBackground::~PageBackground() {
	if (s3eGyroscopeAvailable()) {
		s3eGyroscopeUnRegister(S3E_GYROSCOPE_CALLBACK_DATA_UPDATE, (s3eCallback)GyroscopeCallback);
		s3eGyroscopeStop();
	}
	
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
		CIwSVec2 gyrooffset;
		gyrooffset.x = m_xGyro.m_X * 5.0f;
		gyrooffset.y = m_xGyro.m_Y * 5.0f;
		
        VertexStreamScreen shape;
        const CIwSVec2& screensize = frame.GetScreensize();
        shape.SetRect(CIwRect(gyrooffset.x, gyrooffset.y, screensize.x, screensize.y));
        renderer.Draw(shape, *m_pxBackground);
    }	
}

void PageBackground::SetGyroData(const s3eGyroscopeData& data) {
	m_xGyro = data;
}

void PageBackground::GyroscopeCallback(void* sysdata, void* usrdata) {
	if (PageBackground* bg = (PageBackground*) usrdata) {
		if (s3eGyroscopeData* gyro = (s3eGyroscopeData*) sysdata) {
			bg->SetGyroData(*gyro);
		}
	}
}

