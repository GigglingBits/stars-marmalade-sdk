//--------------------------------------------------------------------------
// Start-up of the application, and event loop
//--------------------------------------------------------------------------
#include "s3e.h"
#include "IwResManager.h"
#include "IwGx.h"
#include "IwGxFont.h"
#include "IwResHandlerWAV.h"
#include "IwUI.h"

#include "SoundEngine.h"
#include "DeviceInfo.h"

#ifdef IW_DEBUG
  #include "Test.h"
#endif

#include "Debug.h"
#include "Stopwatch.h"
#include "App.h"
#include "FrameData.h"
#include "Configuration.h"
#include "InputManager.h"
#include "LogManager.h"
#include "World.h"

//--------------------------------------------------------------------------
// Global helpers
//--------------------------------------------------------------------------
CIwUIView* TheUIView = NULL;
CIwUIController* TheUIController = NULL;

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

	IwUIInit();
	{
		TheUIView =	new CIwUIView();
		TheUIController = new CIwUIController();
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

	{
		delete TheUIView; TheUIView = NULL;
		delete TheUIController; TheUIController = NULL;
	}
	IwUITerminate();

	IwGxTerminate();
}

//--------------------------------------------------------------------------
// Global Main function
//--------------------------------------------------------------------------
void DoLoop() {
	IW_CALLSTACK_SELF;
	App app;
	FrameData frame;

	// application main loop
	while (true) {
		if (s3eDeviceCheckQuitRequest()) {
			break;
		}

		
		// update frame information
		frame.SetScreenSize(
			IwGxGetDisplayWidth(), 
			IwGxGetDisplayHeight());
		frame.Tick();

		// update application state
		app.Update(frame);
		InputManager::GetInstance().ResetButtonState();
		IwGetUIController()->Update(); 
		IwGetUIView()->Update(frame.GetRealDurationMs());
		s3eDeviceYield();

		// render the scene
		IwGxTickUpdate();
		IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);

		app.Render(frame);
		IwGetUIView()->Render();

		IwGxFlush();
		IwUIDebugRender(NULL, IW_UI_DEBUG_LAYOUT_OUTLINE_F | IW_UI_DEBUG_ELEMENT_OUTLINE_F);	
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
