//--------------------------------------------------------------------------
// Start-up of the application, and event loop
//--------------------------------------------------------------------------
#include "s3e.h"
#include "IwResManager.h"
#include "IwGx.h"
#include "IwGxFont.h"
#include "IwResHandlerWAV.h"
#include "SoundEngine.h"
#include "DeviceInfo.h"

#ifdef IW_DEBUG
  #include "Test.h"
#endif

#include "Debug.h"
#include "Stopwatch.h"
#include "App.h"
#include "Configuration.h"
#include "InputManager.h"
#include "LogManager.h"
#include "World.h"

//--------------------------------------------------------------------------
// Global helpers
//--------------------------------------------------------------------------
void Initialize() {
	IW_CALLSTACK_SELF;

	IwGxInit();
	{
		IwGxMipMappingOff();

		IwGxLightingOn(); // required only for IwGxFont colouring
		IwGxSetColClear(0x40, 0x40, 0x40, 0x00);

		CIwMaterial* pMat = IwGxGetMaterialTemplate();
		pMat->SetShadeMode(CIwMaterial::SHADE_GOURAUD);
		pMat->SetCullMode(CIwMaterial::CULL_NONE);
		pMat->SetModulateMode(CIwMaterial::MODULATE_RGB);
		pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		pMat->SetBlendMode(CIwMaterial::BLEND_BLEND);
	}

	IwResManagerInit();
	IwGetResManager()->LoadGroup("sprites.group");

	IwGxFontInit();

	//// flip y axis: https://www.airplaysdk.com/node/3193
	//IwGetGxState()->m_InternalFlags |= IW_GX_INTERNAL_VERTICAL_FLIP_RENDER_F;

	SoundEngine::Initialize();
	Configuration::Initialize();
	InputManager::Initialize();
	DeviceInfo::Initialize();
	LogManager::Initialize();

	World::SetDefaultGravity(-1.5f, 0.0f);

	std::srand((unsigned int)s3eTimerGetUST());
}

void Terminate() {
	IW_CALLSTACK_SELF;

	LogManager::Terminate();
	DeviceInfo::Terminate();
	InputManager::Terminate();
	Configuration::Terminate();
	SoundEngine::Terminate();

	IwGxFontTerminate();
	IwGetResManager()->DestroyGroup("sprites");
	IwResManagerTerminate();
	IwGxTerminate();
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
		SoundEngine::Update();
		s3eDeviceYield();
	}
}

S3E_MAIN_DECL void IwMain() {
	Initialize();
	s3eDeviceYield();

#ifdef IW_DEBUG
	Test::RunTest();
#endif

	DoLoop();

	s3eDeviceYield();
	Terminate();
	s3eDeviceExit();
}
