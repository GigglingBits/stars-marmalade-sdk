#include "DeviceInfo.h"

#include "IwDebug.h"
#include "Debug.h"
#include "s3e.h"
#include "dpiExt.h"

DeviceInfo* DeviceInfo::s_pxInstance = NULL;

void DeviceInfo::Initialize() {
	GetInstance().Update();
}

void DeviceInfo::Terminate() {
	if (s_pxInstance) {
		delete s_pxInstance;
		s_pxInstance = NULL;
	}
}

DeviceInfo::DeviceInfo() {
	LoadConfiguration();
}

DeviceInfo::~DeviceInfo() {
}

DeviceInfo& DeviceInfo::GetInstance() {
	if (!s_pxInstance) {
		s_pxInstance = new DeviceInfo();
	}
	return *s_pxInstance;
}

int DeviceInfo::GetScreenDpi() const {
	return m_xCurrentDevice.ScreenDpi;
}

const std::string& DeviceInfo::GetLongDeviceId() const {
	return m_xCurrentDevice.DescLong;
}

void DeviceInfo::Add(const std::string& id, const std::string desc, uint resolution) {
	Device info;
	info.IdType = eIdentificationLibrary;
	info.Id = id;
	info.Desc = desc;
	info.ScreenDpi = resolution;
	m_xDeviceLibrary[id] = info;
}

void DeviceInfo::Update() {
	// look for device information
	std::string deviceid = s3eDeviceGetString(S3E_DEVICE_ID);
	DeviceMap::iterator it = m_xDeviceLibrary.find(deviceid);
	if (it != m_xDeviceLibrary.end()) {
		// known device
		m_xCurrentDevice = it->second;
	} else if (dpiExtAvailable()) {
		// unknown device; ask dpiExt
		m_xCurrentDevice.IdType = eIdentificationDpiExt;
		m_xCurrentDevice.Id = deviceid;
		m_xCurrentDevice.Desc = "unknown";
		m_xCurrentDevice.ScreenDpi = dpiExtGetDeviceDPI();
	} else {
		// unknown device; just pick some defaults
		m_xCurrentDevice.IdType = eIdentificationUnknown;
		m_xCurrentDevice.Id = deviceid;
		m_xCurrentDevice.Desc = "unknown";
		m_xCurrentDevice.ScreenDpi = 163;
	}
	
	// build long description
	std::string longdesc;
	longdesc += m_xCurrentDevice.Desc;
	longdesc += " (";
	longdesc += m_xCurrentDevice.Id;
	longdesc += ", ";
	if (m_xCurrentDevice.IdType == eIdentificationLibrary) {
		longdesc += "well-known";
	} else if (m_xCurrentDevice.IdType == eIdentificationDpiExt) {
		longdesc += "dpiExt";
	} else {
		longdesc += "unknown";
	}
	longdesc += ")";
	m_xCurrentDevice.DescLong = longdesc;
}

void DeviceInfo::LoadConfiguration() {
	IW_CALLSTACK_SELF;

	Add("iPhone1,2", "iPhone 3G", 163);
	Add("iPhone2,1", "iPhone 3GS", 163);
	Add("iPhone3,1", "iPhone 4", 326);
	Add("iPhone3,3", "Verizon iPhone 4", 326);
	Add("iPhone4,1", "iPhone 4S", 326);
	Add("iPhone5,1", "iPhone 5 (GSM)", 326);
	Add("iPhone5,2", "iPhone 5 (GSM+CDMA)", 326);
	Add("iPhone5,3", "iPhone 5c (GSM)", 326);
	Add("iPhone5,4", "iPhone 5c (GSM+CDMA)", 326);
	Add("iPhone6,1", "iPhone 5s (GSM)", 326);
	Add("iPhone6,2", "iPhone 5s (GSM+CDMA)", 326);
	Add("iPod1,1", "iPod Touch 1G", 163);
	Add("iPod2,1", "iPod Touch 2G", 163);
	Add("iPod3,1", "iPod Touch 3G", 163);
	Add("iPod4,1", "iPod Touch 4G", 326);
	Add("iPod5,1", "iPod Touch 5G", 326);
	Add("iPad1,1", "iPad", 132);
	Add("iPad2,1", "iPad 2 (WiFi)", 132);
	Add("iPad2,2", "iPad 2 (GSM)", 132);
	Add("iPad2,3", "iPad 2 (CDMA)", 132);
	Add("iPad2,4", "iPad 2 (WiFi)", 132);
	Add("iPad2,5", "iPad Mini (WiFi)", 163);
	Add("iPad2,6", "iPad Mini (GSM)", 163);
	Add("iPad2,7", "iPad Mini (GSM+CDMA)", 163);
	Add("iPad3,1", "iPad 3 (WiFi)", 264);
	Add("iPad3,2", "iPad 3 (GSM+CDMA)", 264);
	Add("iPad3,3", "iPad 3 (GSM)", 264);
	Add("iPad3,4", "iPad 4 (WiFi)", 264);
	Add("iPad3,5", "iPad 4 (GSM)", 264);
	Add("iPad3,6", "iPad 4 (GSM+CDMA)", 264);
	Add("iPad4,1", "iPad Air (WiFi)", 264);
	Add("iPad4,2", "iPad Air (Cellular)", 264);
}

