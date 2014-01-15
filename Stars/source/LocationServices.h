#ifndef __LOCATIONSERVICES_H__
#define __LOCATIONSERVICES_H__

#include "s3eGyroscope.h"
#include "s3eSurface.h"

class LocationServices {
private:
	enum AppOrientation {
		eAppOrientUp = 0,
		eAppOrientRight = 90,
		eAppOrientDown = 180,
		eAppOrientLeft = 270,
	};
	
public:
	struct DeviceOrientation {
		float x;
		float y;
		float z;
	};
	
private:
	AppOrientation m_eAppOrientation;
	DeviceOrientation m_xDeviceOrientation;
	
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
	void SetSurfaceData(const s3eSurfaceOrientation& orien);
	static void SurfaceCallback(s3eSurfaceOrientation *orien, void *usrdata);
	
	void SetGyroData(const s3eGyroscopeData& data);
	static void GyroscopeCallback(void* sysdata, void* usrdata);
};

#endif
