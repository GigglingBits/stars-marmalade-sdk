#include "Analytics.h"

#include "IwDebug.h"

Analytics* Analytics::s_pxInstance = NULL;

void Analytics::Initialize(const std::string& appid) {
	if (!s3eFlurryAvailable()) {
        IwTrace(S3EFLURRY, ("Flurry extension not found"));
    } else if (appid.empty()) {
        IwTrace(S3EFLURRY, ("Please supply a valid Flurry Api ID for FLURRY_API_KEY"));
    } else {
        s3eFlurryStartSession(appid.c_str());
    }

	GetInstance();
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
