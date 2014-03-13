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
	m_xVideoView.SetPosition(CIwRect(0, 0, screensize.x, screensize.y));
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
