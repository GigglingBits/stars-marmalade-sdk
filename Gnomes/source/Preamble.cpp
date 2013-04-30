#include "Preamble.h"
#include "Debug.h"
#include "FactoryManager.h"

Preamble::Preamble(const std::string &text) : 
	Page(),
	m_xScreenSize(CIwSVec2::g_Zero) {

	m_pxBackground = FactoryManager::GetTextureFactory().Create("preamble_bg");
	m_sText = text;

	// attach event handlers
	InputManager& im = InputManager::GetInstance();
	im.TouchEndEvent.AddListener<Preamble>(this, &Preamble::TouchEndEventHandler);
	m_xTimer.ElapsedEvent.AddListener<Preamble>(this, &Preamble::TimerElapsedEventHandler);
}

Preamble::~Preamble() {
	// detach event handlers
	InputManager& im = InputManager::GetInstance();
	im.TouchEndEvent.RemoveListener<Preamble>(this, &Preamble::TouchEndEventHandler);
	m_xTimer.ElapsedEvent.RemoveListener<Preamble>(this, &Preamble::TimerElapsedEventHandler);

	if (m_pxBackground) {
		delete m_pxBackground;
	}
}

void Preamble::Initialize() {
	if (m_sText.empty()) {
		SetCompletionState(eCompleted);
		return;
	}
	m_xTimer.Start(4000, 0);
}

void Preamble::OnUpdate(const FrameData& frame) {
	m_xTimer.Update(frame.GetRealDurationMs());

	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
    }
}

void Preamble::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.SetViewport(m_xCamera.GetViewport());

	const CIwSVec2& screensize = frame.GetScreensize();
	CIwRect screenrect(0, 0, screensize.x, screensize.y);

    // background
    if (m_pxBackground) {
        VertexStreamScreen shape;
        shape.SetRect(CIwRect(0, 0, screensize.x, screensize.y));
        renderer.Draw(shape, *m_pxBackground);
    }

	// text
	CIwRect textrect(0, screensize.y / 2, screensize.x, screensize.y / 2);
	renderer.DrawText(m_sText, textrect);
}

void Preamble::TouchEndEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args) {
	SetCompletionState(eCompleted);
}

void Preamble::TimerElapsedEventHandler(const Timer& sender, const long& data) {
	SetCompletionState(eCompleted);
}
