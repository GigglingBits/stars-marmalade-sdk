#include "DeviceInfo.h"
#include "s3e.h"
#include "IwDebug.h"

DeviceInfo DeviceInfo::m_sxInfo;

DeviceInfo::DeviceInfo() {
	m_bInitialized = false;
}

void DeviceInfo::Initialize() {
	IwAssert(MYAPP, !m_sxInfo.m_bInitialized);
	LoadConfiguration(m_sxInfo);
	m_sxInfo.m_bInitialized = true;
}

void DeviceInfo::Terminate() {
	IwAssert(MYAPP, m_sxInfo.m_bInitialized);
}

const DeviceInfo& DeviceInfo::GetInfo() {
	IwAssert(MYAPP, m_sxInfo.m_bInitialized);
	return m_sxInfo;
}

void DeviceInfo::LoadConfiguration(DeviceInfo& info) {
	// sources of information:
	// http://en.wikipedia.org/wiki/List_of_displays_by_pixel_density
	// http://www.madewithmarmalade.com/devnet/forum/ios-device-id-list-3

	int32 deviceid = s3eDeviceGetInt(S3E_DEVICE_ID);
	switch(deviceid) {

	case 0xDE4D7F18: // Apple iPhone4 "iPhone3,1" 
	case 0xDE4D8359: // Apple iPhone4S "iPhone4,1" 
		info.m_iScreenPpcm = 128;	// 326 PPI
		break;

	case 0x58123873: // Apple iPad3 "iPad3,1" 
		info.m_iScreenPpcm = 104;	// 264 PPI
		break;

	case 0xDE4D7697: // Apple iPhone3G "iPhone1,2" 
	case 0xDE4D7AD7: // Apple iPhone3GS "iPhone2,1" 
	case 0x590F86FF: // Apple iTouch 1st gen "iPod1,1" 
	case 0x590F8B40: // Apple iTouch 2nd gen "iPod2,1" 
	case 0x590F8F81: // Apple iPod3 (is that right?)
		info.m_iScreenPpcm = 64;	// 163 PPI
		break;

	case 0x58122FF1: // Apple iPad wifi "iPad1,1" 
	case 0x58123432: // Apple iPad2 "iPad2,1" 
		info.m_iScreenPpcm = 52;	// 132 PPI
		break;

	default:
		info.m_iScreenPpcm = 75;	// just a best guess
	}	
}

int DeviceInfo::GetScreenPpcm() const {
	return m_iScreenPpcm;
}
