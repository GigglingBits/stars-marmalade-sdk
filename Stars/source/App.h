#ifndef __APP_H__
#define __APP_H__

#include "PageManager.h"
#include "Renderer.h"
#include "InputManager.h"
#include "FrameData.h"
#include "Stopwatch.h"
#include "MemoryBuckets.h"

class App {
private:
	PageManager m_xPageManager;
	Renderer m_xRenderer;
	FrameData m_xFrameData;

	Stopwatch m_xStopwatch;
    FrequencyCounter m_xUpdateTime;
    FrequencyCounter m_xRenderTime;

public:
	App();
	~App();
	
	void Update();
	void Render();

private:
	void UnloadLevelData();
	bool LoadLocalLevelData();
	bool LoadOnlineLevelData();

	Renderer& GetRenderer();
	Stopwatch& GetStopwatch();

private:
	void PrintFps(const CIwRect& rect, float realframetime, float simframetime, float pureupdatetime, float purerendertime);
	void PrintMem(const CIwRect& rect, MemoryBuckets bucketid, const char* bucketname);

	void ButtonReleasedEventHandler(const InputManager& sender, const InputManager::ButtonEventArgs& args);
};

#endif
