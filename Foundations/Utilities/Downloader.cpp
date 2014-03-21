#include "Downloader.h"
#include "Debug.h"
#include "s3eMemory.h"
#include "IwGx.h"
#include "IwGxPrint.h"

Downloader::Downloader() : m_eHTTPStatus(kOK), m_ulResultBufLen(0), m_pcResultBuf(NULL) {
}

Downloader::~Downloader() {
	m_xHttp.Cancel();
	s3eFree(m_pcResultBuf);
}

bool Downloader::Download(const std::string& url, std::string& data) {
	IW_CALLSTACK_SELF;

	IwAssertMsg(MYAPP, m_eHTTPStatus == kOK, ("Wrong HTTP status!"));

	if (m_xHttp.Get(url.c_str(), GotHeaders, (void*)this) != S3E_RESULT_SUCCESS) {
		IwAssertMsg(MYAPP, false, ("HTTP GET command failed with status: %u", m_xHttp.GetResponseCode()));
		return false;
	} else {
		m_eHTTPStatus = kDownloading;
	}

	// wait for download to complete or time out
	while (m_eHTTPStatus == kDownloading) {
		s3eDeviceYield(100);
	}

	if (m_eHTTPStatus == kOK) {
		data.assign(m_pcResultBuf, m_ulResultBufLen);
		delete [] m_pcResultBuf;
		m_pcResultBuf = NULL;
		m_ulResultBufLen = 0;
		return true;
	}

	m_xHttp.Cancel();
	m_eHTTPStatus = kOK;
	return false;
}

int32 Downloader::GotHeaders(void* sysdata, void* usrdata) {
	IW_CALLSTACK_SELF;

	// Called when the response headers have been received
	Downloader* dl = (Downloader*) usrdata;
	CIwHTTP& http = dl->m_xHttp;

	if (http.GetStatus() == S3E_RESULT_ERROR) {
		// Something has gone wrong
		dl->m_eHTTPStatus = kError;
	} else {
		// Depending on how the server is communicating the content
		// length, we may actually know the length of the content, or
		// we may know the length of the first part of it, or we may
		// know nothing. ContentExpected always returns the smallest
		// possible size of the content, so allocate that much space
		// for now if it's non-zero. If it is of zero size, the server
		// has given no indication, so we need to guess. We'll guess at 1k.
		dl->m_ulResultBufLen = http.ContentExpected();
		if (!dl->m_ulResultBufLen) {
			dl->m_ulResultBufLen = 1024;
		}

		IwAssertMsg(MYAPP, !dl->m_pcResultBuf, ("There seems to be a buffer allocated already for the HTTP download! How can that be?"));
		s3eFree(dl->m_pcResultBuf);
		dl->m_pcResultBuf = (char*)s3eMalloc(dl->m_ulResultBufLen + 1);
		dl->m_pcResultBuf[dl->m_ulResultBufLen] = 0;
		http.ReadContent(dl->m_pcResultBuf, dl->m_ulResultBufLen, GotData, usrdata);
	}

	return 0;
}

int32 Downloader::GotData(void* sysdata, void* usrdata) {
	IW_CALLSTACK_SELF;

	// This is the callback indicating that a ReadContent call has
	// completed.  Either we've finished, or a bigger buffer is
	// needed.  If the correct ammount of data was supplied initially,
	// then this will only be called once. However, it may well be
	// called several times when using chunked encoding.

	Downloader* dl = (Downloader*) usrdata;
	CIwHTTP& http = dl->m_xHttp;

	// Firstly see if there's an error condition.
	if (http.GetStatus() == S3E_RESULT_ERROR) {
		// Something has gone wrong
		dl->m_eHTTPStatus = kError;
	} else if (http.ContentReceived() != http.ContentLength()) {
		// We have some data but not all of it. We need more space.
		uint32 oldLen = dl->m_ulResultBufLen;
		// If iwhttp has a guess how big the next bit of data is (this
		// basically means chunked encoding is being used), allocate
		// that much space. Otherwise guess.
		if (dl->m_ulResultBufLen < http.ContentExpected()) {
			dl->m_ulResultBufLen = http.ContentExpected();
		} else {
			dl->m_ulResultBufLen += 1024;
		}

		// Allocate some more space and fetch the data.
		dl->m_pcResultBuf = (char*)s3eRealloc(dl->m_pcResultBuf, dl->m_ulResultBufLen);
		http.ReadContent(&dl->m_pcResultBuf[oldLen], dl->m_ulResultBufLen - oldLen, GotData);
	} else {
		// We've got all the data. Display it.
		dl->m_eHTTPStatus = kOK;
	}
	return 0;
}
