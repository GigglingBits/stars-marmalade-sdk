#ifndef __ANALYTICS_H__
#define __ANALYTICS_H__

#include <string>
#include <map>

#include "s3eFlurry.h"

class Analytics {
public:
	typedef std::map<std::string, std::string> Params;
	
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
	
public:
	void Write(const std::string& event);
	void Write(const std::string& event, const Params& params);
};

#endif
