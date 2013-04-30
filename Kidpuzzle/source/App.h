#ifndef __APP_H__
#define __APP_H__

#include "PageManager.h"
#include "Renderer.h"
#include "InputManager.h"
#include "Stopwatch.h"

class App {
private:
	PageManager m_xPageManager;
	Renderer m_xRenderer;

	Stopwatch m_xStopwatch;
    FrequencyCounter m_xUpdateTime;
    FrequencyCounter m_xRenderTime;

public:
	App();
	~App();
	
	void Update(const FrameData& frame);
	void Render(const FrameData& frame);

private:
	void UnloadLevelData();
	bool LoadLocalLevelData();
	bool LoadOnlineLevelData();

	Renderer& GetRenderer();
	Stopwatch& GetStopwatch();

private:
	void PrintFps(const CIwRect& rect, float realframetime, float simframetime, float pureupdatetime, float purerendertime);

	void ButtonReleasedEventHandler(const InputManager& sender, const InputManager::ButtonEventArgs& args);
};

#endif
