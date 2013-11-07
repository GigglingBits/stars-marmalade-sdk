#ifndef __LOCATIONSERVICES_H__
#define __LOCATIONSERVICES_H__

#include "s3eGyroscope.h"

class LocationServices {
private:
	s3eGyroscopeData m_xGyro;

private:
	static LocationServices* s_pxInstance;
	LocationServices();
	~LocationServices();
	
public:
	static void Initialize();
	static void Terminate();
	
	static LocationServices& GetInstance();
	
public:
	const s3eGyroscopeData& GetGyroData();
	
private:
	void SetGyroData(const s3eGyroscopeData& data);
	static void GyroscopeCallback(void* sysdata, void* usrdata);
};

#endif
