#include "IntroMovie.h"
#include "Debug.h"
#include "FactoryManager.h"
#include "MediaViewFactory.h"
#include "Configuration.h"

IntroMovie::IntroMovie(const std::string& videofile) :
	Page("", ""), m_xVideoView(videofile) {
	IW_CALLSTACK_SELF;
		
	// attach event handlers
	InputManager& im = InputManager::GetInstance();
	im.TouchEndEvent.AddListener<IntroMovie>(this, &IntroMovie::TouchEndEventHandler);
}

IntroMovie::~IntroMovie() {
	// detach event handlers
	InputManager& im = InputManager::GetInstance();
	im.TouchEndEvent.RemoveListener<IntroMovie>(this, &IntroMovie::TouchEndEventHandler);
}

void IntroMovie::Initialize() {
	const uint32 BLACK = 0xff000000;
	SetBackground(BLACK, BLACK, BLACK, BLACK);

	m_xVideoView.Finished.AddListener<IntroMovie>(this, &IntroMovie::MediaFinishedEventHandler);
	m_xVideoView.Initialize();
}

void IntroMovie::OnDoLayout(const CIwSVec2& screensize) {
	// determine the size, so that the entire movie fits on the screen
	// the movie is assumed to be 16:9 widescreen
	float aspectratio = 16.0f / 9.0f;
	CIwSVec2 moviesize(screensize);
	moviesize.x = std::min<uint16>(moviesize.x, moviesize.y * aspectratio);
	moviesize.y = std::min<uint16>(moviesize.y, moviesize.x / aspectratio);

	// position the movie in the center
	CIwSVec2 screencenter(screensize.x, screensize.y);
	CIwRect moviearea(
		(screencenter.x - moviesize.x) / 2,
		(screensize.y - moviesize.y) / 2,
		moviesize.x,
		moviesize.y);
	m_xVideoView.SetPosition(moviearea);
}

void IntroMovie::OnUpdate(const FrameData& frame) {
	m_xVideoView.Update(frame);
}

void IntroMovie::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.SetViewport(m_xCamera.GetViewport());

	m_xVideoView.Render(renderer, frame);
}

void IntroMovie::TouchEndEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args) {
	SetCompletionState(eCompleted);
}

void IntroMovie::MediaFinishedEventHandler(const MediaView& sender, const int& args) {
	SetCompletionState(eCompleted);
}
