#ifndef _DOWNLOADER_H_ 
#define _DOWNLOADER_H_ 

#include "IwHTTP.h"

#include <list>

class Downloader {
private:
	enum HTTPStatus {
		kNone,
		kDownloading,
		kOK,
		kError
	};

private:
	std::string m_sLastError;

	CIwHTTP m_xHttp;
	HTTPStatus m_eHTTPStatus;

	uint32 m_ulResultBufLen;
	char* m_pcResultBuf;

public:
	Downloader();
	virtual ~Downloader();

	bool Download(const std::string& url, std::string& data);
	const std::string& GetLastError();

private:
	static int32 GotHeaders(void* sysdata, void* usrdata);
	static int32 GotData(void* sysdata, void* usrdata);
};

#endif
