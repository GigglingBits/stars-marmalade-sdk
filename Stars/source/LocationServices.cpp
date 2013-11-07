#include "LocationServices.h"
#include "Debug.h"

LocationServices* LocationServices::s_pxInstance = NULL;

LocationServices::LocationServices() {
	if (s3eGyroscopeAvailable()) {
		s3eGyroscopeStart();
		s3eGyroscopeSetSensorDelay(DELAY_UI);
		s3eGyroscopeRegister(S3E_GYROSCOPE_CALLBACK_DATA_UPDATE, (s3eCallback)GyroscopeCallback, this);
	}
}

LocationServices::~LocationServices() {
	if (s3eGyroscopeAvailable()) {
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

const s3eGyroscopeData& LocationServices::GetGyroData() {
	return m_xGyro;
}

void LocationServices::SetGyroData(const s3eGyroscopeData& data) {
	m_xGyro = data;
}

void LocationServices::GyroscopeCallback(void* sysdata, void* usrdata) {
	if (LocationServices* ls = (LocationServices*) usrdata) {
		if (s3eGyroscopeData* gyro = (s3eGyroscopeData*) sysdata) {
			ls->SetGyroData(*gyro);
		}
	}
}
