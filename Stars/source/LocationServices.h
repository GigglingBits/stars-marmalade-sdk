#ifndef __LOCATIONSERVICES_H__
#define __LOCATIONSERVICES_H__

#include "s3eGyroscope.h"
#include "s3eSurface.h"

class LocationServices {
public:
	struct DeviceOrientation {
		float x;
		float y;
		float z;
	};
	
private:
	DeviceOrientation m_xOrientation;
	
private:
	static LocationServices* s_pxInstance;
	LocationServices();
	~LocationServices();
	
public:
	static void Initialize();
	static void Terminate();
	
	static LocationServices& GetInstance();
	
public:
	const DeviceOrientation& GetDeviceOrientation();
	
private:
	bool NeedYInversion();
	
	void SetGyroData(const s3eGyroscopeData& data);
	static void GyroscopeCallback(void* sysdata, void* usrdata);
	
//	static void ScreenSizeOrientationChangedCallback(s3eSurfaceOrientation *orien, void *pUserData);
};

#endif
