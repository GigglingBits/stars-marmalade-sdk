#include "LocationServices.h"
#include "Debug.h"

LocationServices* LocationServices::s_pxInstance = NULL;

LocationServices::LocationServices() {
	m_xOrientation.x = 0.0f;
	m_xOrientation.y = 0.0f;
	m_xOrientation.z = 0.0f;
	
	if (s3eGyroscopeAvailable()) {
		s3eGyroscopeStart();
		s3eGyroscopeSetSensorDelay(DELAY_GAME);
		s3eGyroscopeRegister(S3E_GYROSCOPE_CALLBACK_DATA_UPDATE, (s3eCallback)GyroscopeCallback, this);
		
/*
		s3eSurfaceRegister(
			S3E_SURFACE_SCREENSIZE,
			(s3eCallback)ScreenSizeOrientationChangedCallback, this);
*/
	}
}

LocationServices::~LocationServices() {
	if (s3eGyroscopeAvailable()) {
/*
		s3eSurfaceUnRegister(
			S3E_SURFACE_SCREENSIZE,
			(s3eCallback)ScreenSizeOrientationChangedCallback);
*/
		s3eGyroscopeUnRegister(S3E_GYROSCOPE_CALLBACK_DATA_UPDATE, (s3eCallback)GyroscopeCallback);
		s3eGyroscopeStop();
	}
}

void LocationServices::Initialize() {
	GetInstance();
}

void LocationServices::Terminate() {
	if (s_pxInstance) {
		delete s_pxInstance;
		s_pxInstance = NULL;
	}
}

LocationServices& LocationServices::GetInstance() {
	if (!s_pxInstance) {
		s_pxInstance = new LocationServices();
	}
	return *s_pxInstance;
}

const LocationServices::DeviceOrientation& LocationServices::GetDeviceOrientation() {
	return m_xOrientation;
}

bool LocationServices::NeedYInversion() {
	int dir = s3eSurfaceGetInt(S3E_SURFACE_BLIT_DIRECTION);
	return S3E_SURFACE_BLIT_DIR_ROT180 == dir || S3E_SURFACE_BLIT_DIR_ROT270 == dir;
}

void LocationServices::SetGyroData(const s3eGyroscopeData& data) {
	// fade the old data
	m_xOrientation.x *= 0.98f;
	m_xOrientation.y *= 0.98f;
	m_xOrientation.z *= 0.98f;
		
	// apply new data
	m_xOrientation.x += data.m_X;
	m_xOrientation.y += NeedYInversion() ? -data.m_Y :  data.m_Y;
	m_xOrientation.z += data.m_Z;
	
	// compensate for app rotation
}

void LocationServices::GyroscopeCallback(void* sysdata, void* usrdata) {
	if (LocationServices* ls = (LocationServices*) usrdata) {
		if (s3eGyroscopeData* gyro = (s3eGyroscopeData*) sysdata) {
			ls->SetGyroData(*gyro);
		}
	}
}

/*
 void LocationServices::ScreenSizeOrientationChangedCallback(s3eSurfaceOrientation *orien, void *pUserData) {
	
}
*/