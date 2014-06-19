//--------------------------------------------------------------------------
// Start-up of the application, and event loop
//--------------------------------------------------------------------------
#include "Main.h"
#include "s3e.h"
#include "IwGx.h"
#include "IwGxFont.h"
#include "IwResHandlerWAV.h"
#include "SpineResource.h"
#include "SoundEngine.h"
#include "ResourceManager.h"
#include "Analytics.h"
#include "LocationServices.h"
#include "DeviceInfo.h"
#include "MarmaladeVersion.h"
#include "dpiExt.h"

#ifdef IW_DEBUG
  #include "Test.h"
#endif

#include "Debug.h"
#include "Stopwatch.h"
#include "App.h"
#include "Configuration.h"
#include "UserSettings.h"
#include "InputManager.h"
#include "LogManager.h"
#include "World.h"

#include <sstream>

//--------------------------------------------------------------------------
// Global helpers
//--------------------------------------------------------------------------
void UpdateScreen() {
	IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
	IwGxTickUpdate();
	IwGxFlush();
	IwGxSwapBuffers();
}

void WriteAndShowLog(const std::string& message, bool clear = false) {
	static std::string s_Messages;

	if (clear) {
		s_Messages.clear();
	}

	if (!Configuration::GetInstance().ShowStats) {
		return;
	}
	
	if (!message.empty()) {
		std::ostringstream oss;
		oss << s_Messages << message << std::endl;
		s_Messages = oss.str();
		
		IwGxPrintString(50, 50, s_Messages.c_str());

		UpdateScreen();
	}
}

void PrintHeader() {
	std::ostringstream oss;
	oss << "*************************************************************" << std::endl;
	oss << "Stars " << std::endl;
	oss << "*************************************************************" << std::endl;
	oss << "Device name:   " << s3eDeviceGetString(S3E_DEVICE_NAME) << std::endl;
	oss << "Device class:  " << s3eDeviceGetString(S3E_DEVICE_CLASS) << std::endl;
	oss << "Device ID:     " << DeviceInfo::GetInstance().GetLongDeviceId() << std::endl;
	oss << "Display:       " << IwGxGetDeviceWidth() << "x" << IwGxGetDeviceHeight() << std::endl;
	oss << "Architecture:  " << s3eDeviceGetString(S3E_DEVICE_ARCHITECTURE) << std::endl;
	oss << "*************************************************************" << std::endl;
	oss << "OS:            " << s3eDeviceGetString(S3E_DEVICE_OS) << " " <<  s3eDeviceGetString(S3E_DEVICE_OS_VERSION) << std::endl;
	oss << "SDK:           " << MARMALADE_VERSION_STRING_FULL << std::endl;
	oss << "Locale:        " << s3eDeviceGetString(S3E_DEVICE_LOCALE) << std::endl;
	oss << "*************************************************************";
	WriteAndShowLog(oss.str());
}

void Initialize() {
	IW_CALLSTACK_SELF;

	IwGxInit();
	{
		IwGxLightingOn(); // required only for IwGxFont colouring
		
		CIwMaterial* pMat = IwGxGetMaterialTemplate();
		pMat->SetShadeMode(CIwMaterial::SHADE_GOURAUD);
		pMat->SetCullMode(CIwMaterial::CULL_NONE);
		pMat->SetModulateMode(CIwMaterial::MODULATE_RGB);
		pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		pMat->SetBlendMode(CIwMaterial::BLEND_BLEND);
		
		IwGxSetColClear(0x20, 0x20, 0x20, 0x00);
	}
	DeviceInfo::Initialize();
		
	IW_CLASS_REGISTER(SpineResource);
	Configuration::Initialize();
	UserSettings::Initialize(Configuration::GetInstance().SettingsFile);
	
	PrintHeader();

	WriteAndShowLog("Initializing foundation...");
	IwResManagerInit();
	IwGxFontInit();

	//// flip y axis: https://www.airplaysdk.com/node/3193
	//IwGetGxState()->m_InternalFlags |= IW_GX_INTERNAL_VERTICAL_FLIP_RENDER_F;

	WriteAndShowLog("Initializing game frameworks...");
	long size = Configuration::GetInstance().ResourceHeapSize * 1000000L;
	IwMemBucketInit();
	IwMemBucketCreate(eMemoryBucketResources, "resources", size);
	ResourceManager::Initialize(eMemoryBucketResources);

	Analytics::Initialize(Configuration::GetInstance().FlurryKey);
	SoundEngine::Initialize();
	InputManager::Initialize();
	LogManager::Initialize();
	LocationServices::Initialize();
	World::SetDefaultGravity(1.0f, -9.8f);

	WriteAndShowLog("Loading resources...");
	ResourceManager::GetInstance().LoadPermament("base.group");
	
	std::srand((unsigned int)s3eTimerGetUST());
}

void Terminate() {
	IW_CALLSTACK_SELF;

	LocationServices::Terminate();
	LogManager::Terminate();
	InputManager::Terminate();
	SoundEngine::Terminate();
	Analytics::Terminate();
	ResourceManager::Terminate();

	IwGxFontTerminate();
	IwResManagerTerminate();

	UserSettings::Terminate();
	Configuration::Terminate();
	IW_CLASS_REMOVE(SpineResource);
	DeviceInfo::Terminate();

	IwGxTerminate();

	IwMemBucketFree(eMemoryBucketResources);
}

//--------------------------------------------------------------------------
// Global Main function
//--------------------------------------------------------------------------
void DoLoop() {
	IW_CALLSTACK_SELF;
	App app;

	// application main loop
	while (true) {
		if (s3eDeviceCheckQuitRequest()) {
			break;
		}

		// update application state
		app.Update();
		InputManager::GetInstance().ResetButtonState();
		s3eDeviceYield();

		// render the GUI
		IwGxTickUpdate();
		IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
		app.Render();
		IwGxFlush();
		IwGxSwapBuffers();
		s3eDeviceYield();

		// clear sound channels
		SoundEngine::GetInstance().Update();
		s3eDeviceYield();
	}
}

S3E_MAIN_DECL void IwMain() {
	Initialize();
	s3eDeviceYield();

#ifdef IW_DEBUG
	WriteAndShowLog("Running tests...");
	Test::RunTest();
#endif

	WriteAndShowLog("", true);

	DoLoop();

	s3eDeviceYield();
	Terminate();
	s3eDeviceExit();
}
