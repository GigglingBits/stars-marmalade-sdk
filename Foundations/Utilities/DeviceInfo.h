#ifndef __DEVICEINFO_H__
#define __DEVICEINFO_H__

#include <string>
#include <map>

class DeviceInfo {
public:
	enum IdentificationType {
		eIdentificationLibrary,
		eIdentificationDpiExt,
		eIdentificationUnknown
	};

public:
	struct Device {
		IdentificationType IdType;
		std::string Id;
		std::string Desc;
		std::string DescLong;
		uint PixelsPerInch;
	};
	typedef std::map<std::string, Device> DeviceMap;
	DeviceMap m_xDeviceLibrary;
	
	Device m_xCurrentDevice;
	
private:
	static DeviceInfo* s_pxInstance;
	DeviceInfo();
	~DeviceInfo();

public:
	static void Initialize();
	static void Terminate();
	static DeviceInfo& GetInstance();

public:
	int GetPixelsPerInch() const;
	const std::string& GetLongDeviceId() const;

private:
	void LoadConfiguration();
	void Add(const std::string& id, const std::string desc, uint resolution);
	void Update();
};

#endif
