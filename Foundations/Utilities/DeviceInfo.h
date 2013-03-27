#ifndef __DEVICEINFO_H__
#define __DEVICEINFO_H__

class DeviceInfo {
private:
	static DeviceInfo m_sxInfo;
	bool m_bInitialized;

	int m_iScreenPpcm;

public:
	DeviceInfo();

	static void Initialize();
	static void Terminate();

	const static DeviceInfo& GetInfo();

	int GetScreenPpcm() const;

private:
	static void LoadConfiguration(DeviceInfo& info);
};

#endif
