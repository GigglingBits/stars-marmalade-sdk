#ifndef __ADVERTS_H__
#define __ADVERTS_H__

#include <string>
#include <map>

#include "s3eFlurryAppSpot.h"
#include "s3eFlurry.h"

#include "MulticastEvent.h"

class Adverts {
public:
	typedef std::map<std::string, std::string> Params;
	
private:
	static Adverts* s_pxInstance;
	Adverts();
	~Adverts();
	
private:
	std::string m_sFlurryAppId;
	
public:
	static void Initialize(const std::string& appid);
	static void Terminate();
	static Adverts& GetInstance();
	
public:
	void Show();
	
public:
	MulticastEvent<Adverts, int> AdvertClosed;
	
private:
	static int32 OnAdvertClosed(void* systemData, void* userData);
};

#endif
