#ifndef __LOGMANAGER_H__
#define __LOGMANAGER_H__

#include <string>

class LogManager {
private:
	static LogManager* s_pxInstance;

private:
	LogManager();
	~LogManager();

public:
	static void Initialize();
	static void Terminate();

	static LogManager& GetInstance();

	void WriteMessage(const std::string& msg);

private:
	static int32 DialogFinishedCallback(void* sysdata, void* usrdata);
};

#endif
