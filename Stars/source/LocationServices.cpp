#include "LocationServices.h"
#include "IwDebug.h"
#include "Debug.h"

LocationServices* LocationServices::s_pxInstance = NULL;

LocationServices::LocationServices() {
	m_xDeviceOrientation.x = 0.0f;
	m_xDeviceOrientation.y = 0.0f;
	m_xDeviceOrientation.z = 0.0f;
	
	if (s3eGyroscopeAvailable()) {
		s3eGyroscopeStart();
		s3eGyroscopeSetSensorDelay(DELAY_UI);
		s3eGyroscopeRegister(S3E_GYROSCOPE_CALLBACK_DATA_UPDATE, (s3eCallback)GyroscopeCallback, this);
		
		s3eSurfaceRegister(
			S3E_SURFACE_SCREENSIZE,
			(s3eCallback)SurfaceCallback, this);
	}
}

LocationServices::~LocationServices() {
	if (s3eGyroscopeAvailable()) {

		s3eSurfaceUnRegister(
			S3E_SURFACE_SCREENSIZE,
			(s3eCallback)SurfaceCallback);

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
	return m_xDeviceOrientation;
}

void LocationServices::SetGyroData(const s3eGyroscopeData& data) {
	
	IwTrace(LOCATIONSERVICES, ("Gyro data received: x=%.4f y=%.4f z=%.4f", data.m_X, data.m_Y, data.m_Z));
	
	if (data.m_X == 0.0f && data.m_Y == 0.0f && data.m_Z == 0.0f) {
		// no rotation detected; ignore callback
		return;
	}
	
	// fade the old data
	m_xDeviceOrientation.x *= 0.98f;
	m_xDeviceOrientation.y *= 0.98f;
	m_xDeviceOrientation.z *= 0.98f;
	
	// compensate for screen rotation
	float dx, dy, dz;
	if (eAppOrientRight == m_eAppOrientation) {
		// notimplemented; todo: rotate the axis correctly
		dx = data.m_X * 0.0f;
		dy = data.m_Y * 0.0f;
		dz = data.m_Z;
	} else if (eAppOrientDown == m_eAppOrientation) {
		dx = data.m_X;
		dy = data.m_Y;
		dz = data.m_Z;
	} else if (eAppOrientLeft == m_eAppOrientation) {
		// notimplemented; todo: rotate the axis correctly
		dx = data.m_X * 0.0f;
		dy = data.m_Y * 0.0f;
		dz = data.m_Z;
	} else {
		dx = -data.m_X;
		dy = -data.m_Y;
		dz = data.m_Z;
	}
	
	// apply new data
	m_xDeviceOrientation.x += dx;
	m_xDeviceOrientation.y += dy;
	m_xDeviceOrientation.z += dz;
}

void LocationServices::GyroscopeCallback(void* sysdata, void* usrdata) {
	if (LocationServices* ls = (LocationServices*) usrdata) {
		if (s3eGyroscopeData* gyro = (s3eGyroscopeData*) sysdata) {
			ls->SetGyroData(*gyro);
		}
	}
}

void LocationServices::SetSurfaceData(const s3eSurfaceOrientation& orien) {
	if (orien.m_OrientationChanged) {
		return;
	}
	
	switch (orien.m_DeviceBlitDirection) {
		case S3E_SURFACE_BLIT_DIR_NORMAL: {
			m_eAppOrientation = eAppOrientUp;
			break;
		}
		case S3E_SURFACE_BLIT_DIR_ROT90: {
			m_eAppOrientation = eAppOrientLeft;
			break;
		}
		case S3E_SURFACE_BLIT_DIR_ROT180: {
			m_eAppOrientation = eAppOrientDown;
			break;
		}
		case S3E_SURFACE_BLIT_DIR_ROT270: {
			m_eAppOrientation = eAppOrientLeft;
			break;
		}
		default: {
			m_eAppOrientation = eAppOrientUp;
		}
	}
}

void LocationServices::SurfaceCallback(s3eSurfaceOrientation *orien, void *usrdata) {
	if (LocationServices* ls = (LocationServices*) usrdata) {
		if (orien) {
			ls->SetSurfaceData(*orien);
		}
	}}
