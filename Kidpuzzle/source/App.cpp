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

void App::Update(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	Stopwatch& watch = GetStopwatch();
	watch.Reset();
	watch.Start();

	// execute one frame
	InputManager::GetInstance().Update();
	m_xPageManager.Update(frame);

	m_xUpdateTime.Tick(watch.GetElapsed());
}

void App::Render(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	Stopwatch& watch = GetStopwatch();
	watch.Reset();
	watch.Start();

	CIwSVec2 screensize(
		IwGxGetDisplayWidth(),
		IwGxGetDisplayHeight());

	m_xPageManager.Render(GetRenderer(), frame);

	m_xRenderTime.Tick(watch.GetElapsed());
	
    // fps for debug...
	const int margin = 5;
	CIwRect rect(margin, margin, screensize.x - (2 * margin), margin);
	PrintFps(rect,
		frame.GetAvgRealDurationMs(),
		frame.GetAvgSimulatedDurationMs(),
        m_xUpdateTime.GetAvgTickTimeMs(),
        m_xRenderTime.GetAvgTickTimeMs());
}

void App::PrintFps(const CIwRect& rect, float realframetime, float simframetime, float pureupdatetime, float purerendertime) {
	IW_CALLSTACK_SELF;

	int fps = (int)(1000.0 / realframetime);
	float simratio = simframetime / realframetime;
	
	const int len = 80;
	char buf[len];
	snprintf(buf, len, "fps: %i; sim-ratio: %.0f%%;\nupdate: %.0f%%; render: %.0f%%; system: %.0f%%", 
		fps, 100.0f * simratio, 
		100.0f * pureupdatetime / realframetime, 
		100.0f * purerendertime / realframetime, 
		100.0f * (realframetime - pureupdatetime - purerendertime) / realframetime);

	GetRenderer().DrawText(
		std::string(buf), rect, 
		Renderer::eFontTypeSystem, 
		simratio > 0.95f ? 0xffffffff : 0xff0000ff);
}

void App::ButtonReleasedEventHandler(const InputManager& sender, const InputManager::ButtonEventArgs& args) {
	IW_CALLSTACK_SELF;
	if  (args.cmdid == eButtonCommandIdNextLevel) {
		m_xPageManager.NextLevel();

	} else if (args.cmdid == eButtonCommandIdRestartLevel) {
		m_xPageManager.RestartLevel();

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

		m_xPageManager.RestartLevel();

	} else if (args.cmdid == eButtonCommandIdOpenLevelMenu) {
		m_xPageManager.StartLevelMenu();

	} else if (args.cmdid == eButtonCommandIdOpenLevel) {
		m_xPageManager.StartLevel((int)args.userdata);
	}
}
