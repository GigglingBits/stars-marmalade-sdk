#include "Analytics.h"

#include "s3eFlurry.h"
#include "IwDebug.h"

#include <sstream>

Analytics* Analytics::s_pxInstance = NULL;

void Analytics::Initialize(const std::string& appid) {
	if (!s3eFlurryAvailable()) {
        IwTrace(S3EFLURRY, ("Flurry extension not found"));
		return;
    }
	if (appid.empty()) {
        IwTrace(S3EFLURRY, ("Please supply a valid Flurry Api ID for FLURRY_API_KEY"));
		return;
    }
	
	s3eFlurryStartSession(appid.c_str());
}

void Analytics::Terminate() {
	if (s_pxInstance) {
		delete s_pxInstance;
		s_pxInstance = NULL;
	}
	
	if (s3eFlurryAvailable()) {
        s3eFlurryEndSession();
    }
}

Analytics::Analytics() {
}

Analytics::~Analytics() {
}

Analytics& Analytics::GetInstance() {
	if (!s_pxInstance) {
		s_pxInstance = new Analytics();
	}
	return *s_pxInstance;
}

void Analytics::Log(const std::string& event) {
	s3eFlurryLogEvent(event.c_str());
}

void Analytics::Log(const std::string& event, const Params& params) {
	std::ostringstream oss;
	for (Params::const_iterator it = params.begin(); it != params.end(); it++) {
		if (it != params.begin()) {
			oss << '|';
		}
		oss << it->first << '|' << it->second;
	}
	s3eFlurryLogEventParams(event.c_str(), oss.str().c_str());
}

void Analytics::LogError(const std::string& message) {
	s3eFlurryLogError("Runtime Error", message.c_str());
}
