#include <sstream>

#include "App.h"
#include "InputManager.h"

#include "Configuration.h"
#include "FactoryManager.h"
#include "TextureFactory.h"
#include "BodyFactory.h"
#include "LevelFactory.h"
#include "Downloader.h"
#include "LogManager.h"

#include "Debug.h"

App::App() {
	LoadLocalLevelData();

	// attach event handlers
	InputManager& im = InputManager::GetInstance();
	im.ButtonReleasedEvent.AddListener<App>(this, &App::ButtonReleasedEventHandler);
	
	// set the rendeer up
	m_xRenderer.SetFonts("font_large", "font_normal", "font_small", "font_system");
}

App::~App() {
	// detach event handlers
	InputManager& im = InputManager::GetInstance();
	im.ButtonReleasedEvent.RemoveListener<App>(this, &App::ButtonReleasedEventHandler);

	UnloadLevelData();
}

bool App::LoadOnlineLevelData() {
	IW_CALLSTACK_SELF;

	// define files
	Configuration& cfg = Configuration::GetInstance();

	// download files
	bool success = false;
	Downloader dl;
	std::string data;
	std::list<std::string> datalist;
	if (dl.Download(cfg.HttpBodiesFile, data)) {
		datalist.push_back(data);
		if (dl.Download(cfg.HttpLevelsFile, data)) {
			datalist.push_back(data);
			success = true;
		}
	}

	// try to load
	if (success) {
		success = FactoryManager::InitializeFromStrings(datalist);
	}

	return success;
}

bool App::LoadLocalLevelData() {
	IW_CALLSTACK_SELF;

	std::list<std::string> filenames;
	filenames.push_back(Configuration::GetInstance().BodiesFile);
	filenames.push_back(Configuration::GetInstance().LevelsFile);
	return FactoryManager::InitializeFromFiles(filenames);
}

void App::UnloadLevelData() {
	IW_CALLSTACK_SELF;
	FactoryManager::Terminate();
}

Renderer& App::GetRenderer() {
	return m_xRenderer;
}

Stopwatch& App::GetStopwatch() {
	return m_xStopwatch;
}

void App::Update() {
	IW_CALLSTACK_SELF;

	Stopwatch& watch = GetStopwatch();
	watch.Reset();
	watch.Start();

	// update frame information
	m_xFrameData.SetScreenSize(
		IwGxGetDisplayWidth(), 
		IwGxGetDisplayHeight());
	m_xFrameData.Tick();

	// execute one frame
	InputManager::GetInstance().Update();
	m_xPageManager.Update(m_xFrameData);

	m_xUpdateTime.Tick(watch.GetElapsed());
}

void App::Render() {
	IW_CALLSTACK_SELF;

	Stopwatch& watch = GetStopwatch();
	watch.Reset();
	watch.Start();

	CIwSVec2 screensize(
		IwGxGetDisplayWidth(),
		IwGxGetDisplayHeight());

	m_xPageManager.Render(GetRenderer(), m_xFrameData);

	m_xRenderTime.Tick(watch.GetElapsed());

	if (Configuration::GetInstance().ShowStats) {
		const int margin = 3;
		const int height = 25;
		CIwRect rect(margin, margin, screensize.x - (2 * margin), height);
		PrintFps(rect,
			 m_xFrameData.GetAvgRealDurationMs(),
			 m_xFrameData.GetAvgSimulatedDurationMs(),
			 m_xUpdateTime.GetAvgTickTimeMs(),
			 m_xRenderTime.GetAvgTickTimeMs());

		rect.y += height;
		PrintMem(rect, eMemoryBucketApp);

		rect.y += height;
		PrintMem(rect, eMemoryBucketResources);
	}
}

void App::PrintFps(const CIwRect& rect, float realframetime, float simframetime, float pureupdatetime, float purerendertime) {
	IW_CALLSTACK_SELF;

	int fps = (int)(1000.0 / realframetime);
	float simratio = simframetime / realframetime;
	
	std::ostringstream oss;
	oss.precision(1);
	oss << std::fixed;
	oss << "fps: " << fps << " / ";
	oss << "sim-ratio: " << 100.0f * simratio << std::endl;
	oss << "update: " << 100.0f * pureupdatetime / realframetime << '%' << " / ";
	oss << "render: " << 100.0f * purerendertime / realframetime << '%' << " / ";
	oss << "system: " << 100.0f * (realframetime - pureupdatetime - purerendertime) / realframetime  << '%' << std::endl;

	GetRenderer().DrawText(
		oss.str(), rect,
		Renderer::eFontTypeSystem, 
		simratio > 0.95f ? 0xffffffff : 0xff0000ff);
}

void App::PrintMem(const CIwRect& rect, MemoryBuckets bucketid) {
	IW_CALLSTACK_SELF;

	uint32 usedmem = IwMemBucketGetUsed(bucketid);
	uint32 freemem = IwMemBucketGetFree(bucketid);
	uint32 largestfreeblock = IwMemBucketGetLargestFreeBlock(bucketid);
	
	float mem = 100.0f * usedmem / (usedmem + freemem);
	float lfb = largestfreeblock / 1024.f / 1024.f;
	
	std::ostringstream oss;
	oss.precision(1);
	oss << std::fixed;
	oss << "mem: " << mem << "% / LFB: " << lfb << "MB" << std::endl;

	GetRenderer().DrawText(
		oss.str(), rect,
		Renderer::eFontTypeSystem,
		mem > 0.80f ? 0xffffffff : 0xff0000ff);
}

void App::ButtonReleasedEventHandler(const InputManager& sender, const InputManager::ButtonEventArgs& args) {
	IW_CALLSTACK_SELF;
	if  (args.cmdid == eButtonCommandIdOpenNextLevel) {
		m_xPageManager.StartNextLevel();

	} else if (args.cmdid == eButtonCommandIdRestartLevel) {
		m_xPageManager.StartLevel();

	} else if (args.cmdid == eButtonCommandIdReload) {
		// reload all configuration and
		// start current page over again
		UnloadLevelData();

		bool loaded = false;
		bool useinternet = args.userdata == 1;
		if (useinternet) {
			loaded = LoadOnlineLevelData();
			if (!loaded) { 
				std::string msg = "FAILURE! An error occurred while downloading or installing new game definitions. The packaged game definitions will be re-loaded instead.";
				LogManager::GetInstance().WriteMessage(msg);
				UnloadLevelData();
			}
		}
		
		if (!loaded) {
			loaded = LoadLocalLevelData();
			if (!loaded) { 
				std::string msg = "FAILURE! An error occurred while re-loading local level definitions. The game may not function properly.";
				LogManager::GetInstance().WriteMessage(msg);
			}
		}

		m_xPageManager.StartLevel();

	} else if (args.cmdid == eButtonCommandIdOpenTitleScreen) {
		m_xPageManager.StartTitleScreen();
        
	} else if (args.cmdid == eButtonCommandIdOpenWorldMenu) {
		m_xPageManager.StartWorldMenu();
        
	} else if (args.cmdid == eButtonCommandIdOpenLevelMenu) {
        m_xPageManager.SetWorld((LevelIndexer::WorldId)args.userdata);
		m_xPageManager.StartLevelMenu();
        
	} else if (args.cmdid == eButtonCommandIdOpenLevel) {
        m_xPageManager.SetLevel((int)args.userdata);
		m_xPageManager.StartLevel();

	} else if (args.cmdid == eButtonCommandIdQuitLevel) {
		m_xPageManager.StartLevelMenu();
	}
}
