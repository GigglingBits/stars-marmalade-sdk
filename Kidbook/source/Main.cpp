//--------------------------------------------------------------------------
// Start-up of the application, and event loop
//--------------------------------------------------------------------------
#include "s3e.h"
#include "Iw2D.h"
#include "IwUI.h"

//#include "MemBuckets.h"
//#include "IwResManager.h"

#include "Configuration.h"
#include "Application.h"
#include "Stopwatch.h"
#include "ThreadPool.h"

#ifdef IW_DEBUG
  #include "Test.h"
#endif

//--------------------------------------------------------------------------
// Global application object
//--------------------------------------------------------------------------
Application* TheApp = NULL; 
CIwUIView* TheUIView = NULL;
CIwUIController* TheUIController = NULL;

//--------------------------------------------------------------------------
// Global helpers
//--------------------------------------------------------------------------
void UnInitialize() {
	if (TheApp) {
		delete TheApp;
		TheApp = NULL;
	}

	ThreadPool::Terminate();

	if (Configuration::GetInstance().AccelerometerStarted) {
		s3eAccelerometerStop();
		Configuration::GetInstance().AccelerometerStarted = false;
	}

	if(Configuration::GetInstance().EnableText) {
		IwGetResManager()->DestroyGroup("Fonts");
		IwGxFontTerminate();
	}

	Configuration::UnInitialize();

	{
		if (TheUIController) {
			delete TheUIController;
			TheUIController = NULL;
		}
		if (TheUIView) {
			delete TheUIView;
			TheUIView = NULL;
		}
	}
	IwUITerminate();

	Iw2DTerminate();
}

void Initialize() {
	Iw2DInit();
	{
		IwGxLightingOn();		// required only for IwGxFont colouring
		// IwGxMipMappingOff();	// makes scrolling smoother, since creation of mipmaps takes quite some time between frames

		CIwMaterial* pMat = IwGxGetMaterialTemplate();
		pMat->SetShadeMode(CIwMaterial::SHADE_GOURAUD);
		pMat->SetCullMode(CIwMaterial::CULL_NONE);
		pMat->SetModulateMode(CIwMaterial::MODULATE_RGB);
		pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		pMat->SetBlendMode(CIwMaterial::BLEND_BLEND);

		Iw2DSetUseMipMapping(false); 
	}

	IwUIInit();
	{
		TheUIView =	new CIwUIView();
		TheUIController = new CIwUIController();
	}

	Configuration::Initialize();

	if (Configuration::GetInstance().EnableText) {
		IwGxFontInit();
		IwGetResManager()->LoadGroup("fonts.group");
		CIwGxFont* pFont = (CIwGxFont*)IwGetResManager()->GetResNamed("Font", "CIwGxFont");
		IwGxFontSetFont(pFont);
		IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_MIDDLE);
	}

	if (s3eAccelerometerGetInt(S3E_ACCELEROMETER_AVAILABLE)) {
		Configuration::GetInstance().AccelerometerStarted = S3E_RESULT_SUCCESS == s3eAccelerometerStart();
	}

	ThreadPool::Initialize();

	TheApp = new Application(
		Configuration::GetInstance().ShowStats, 
		Configuration::GetInstance().AutoRotate);
}

//--------------------------------------------------------------------------
// Global Main function
//--------------------------------------------------------------------------
S3E_MAIN_DECL void IwMain() {
	//IwUtilInit();
	//uint32 bucket = IwMemBucketGetID();
	//uint32 checkpoint = IwMemBucketCheckpoint();

	// init frameworks
	Initialize();

#ifdef IW_DEBUG
	//Test::RunTest();
#endif

	// application main loop
	while (!s3eDeviceCheckQuitRequest()) {
		// update application logic
		TheApp->Update();
		s3eDeviceYield();

		//Render the UI
		Iw2DSurfaceClear(0xFFFFFFFF);
		TheApp->Render();
		Iw2DSurfaceShow();
		s3eDeviceYield();
	}

	// application exit is done via callbacks
	UnInitialize();

	// memory debugging
	//IwMemBucketDebugCheck(bucket, checkpoint, NULL);
	//IwUtilTerminate();
}
