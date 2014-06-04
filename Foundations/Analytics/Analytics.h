#ifndef __ANALYTICS_H__
#define __ANALYTICS_H__

#include <string>

#include "s3eFlurry.h"

class Analytics {
private:
	static Analytics* s_pxInstance;
	Analytics();
	~Analytics();
	
private:
	std::string m_sFlurryAppId;
	
public:
	static void Initialize(const std::string& appid);
	static void Terminate();
	static Analytics& GetInstance();
};

#endif
