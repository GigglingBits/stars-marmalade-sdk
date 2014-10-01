#include "Adverts.h"

#include <sstream>

#include "IwDebug.h"

Adverts* Adverts::s_pxInstance = NULL;

void Adverts::Initialize(const std::string& appid) {
	if (!s3eFlurryAppSpotAvailable()) {
        IwTrace(S3EFLURRY, ("Flurry App Spot extension not found"));
		return;
    }
	if (appid.empty()) {
        IwTrace(S3EFLURRY, ("Please supply a valid Flurry Api ID for FLURRY_API_KEY"));
		return;
    }
	
	s3eFlurryAppSpotInitialize(appid.c_str());
}

void Adverts::Terminate() {
	if (s_pxInstance) {
		delete s_pxInstance;
		s_pxInstance = NULL;
	}
}

Adverts::Adverts() {
	s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADCLOSED, Adverts::OnAdvertClosed, this);
	s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADSPACE_RECEIVED, Adverts::OnAdvertClosed, this);
	s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_APPLICATION_EXIT, Adverts::OnAdvertClosed, this);
	s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADSPACE_RECEIVED_FAIL, Adverts::OnAdvertClosed, this);
	s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADCLICKED, Adverts::OnAdvertClosed, this);
	s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADOPENED, Adverts::OnAdvertClosed, this);
	s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADVIDEOCOMPLETED, Adverts::OnAdvertClosed, this);
	s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADRENDERFAIL, Adverts::OnAdvertClosed, this);
}

Adverts::~Adverts() {
	s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADRENDERFAIL, Adverts::OnAdvertClosed);
	s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADCLOSED, Adverts::OnAdvertClosed);
	s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADVIDEOCOMPLETED, Adverts::OnAdvertClosed);
	s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADOPENED, Adverts::OnAdvertClosed);
	s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADCLICKED, Adverts::OnAdvertClosed);
	s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADSPACE_RECEIVED_FAIL, Adverts::OnAdvertClosed);
	s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_APPLICATION_EXIT, Adverts::OnAdvertClosed);
	s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADSPACE_RECEIVED, Adverts::OnAdvertClosed);
}

Adverts& Adverts::GetInstance() {
	if (!s_pxInstance) {
		s_pxInstance = new Adverts();
	}
	return *s_pxInstance;
}

void Adverts::Show() {
	s3eFlurryAppSpotFetchAndDisplayAdForSpace("MyAdSpace_AdSpace_Fullscreen", S3E_FLURRYAPPSPOT_FULLSCREEN);
	s3eFlurryAppSpotFetchAndDisplayAdForSpace("MyAdSpace_AdSpace_Top", S3E_FLURRYAPPSPOT_BANNER_TOP);
	s3eFlurryAppSpotFetchAndDisplayAdForSpace("MyAdSpace_AdSpace_Bottom", S3E_FLURRYAPPSPOT_BANNER_BOTTOM);
}

int32 Adverts::OnAdvertClosed(void* systemData, void* userData) {
	
	
	return 0;
}
