#include "Preamble.h"
#include "Debug.h"
#include "FactoryManager.h"

bool Preamble::NullMediaView::IsShowing() {
	return false;
}

void Preamble::NullMediaView::Show(const CIwVec2& pos, const CIwVec2& size) {
}

void Preamble::NullMediaView::Hide() {
}

Preamble::Preamble(const std::string &text, const std::string& file) : 
	Page(), m_pxMediaView(&m_xNull) {

	m_sText = text;

	m_pxBackground = FactoryManager::GetTextureFactory().Create("preamble_bg");

	m_pxMediaView = &SelectMediaView(file);
	m_pxMediaView->SetFile(file);
	m_pxMediaView->Finished.AddListener<Preamble>(this, &Preamble::MediaFinishedEventHandler);

	// attach event handlers
	InputManager& im = InputManager::GetInstance();
	im.TouchEndEvent.AddListener<Preamble>(this, &Preamble::TouchEndEventHandler);
}

Preamble::~Preamble() {
	// detach event handlers
	InputManager& im = InputManager::GetInstance();
	im.TouchEndEvent.RemoveListener<Preamble>(this, &Preamble::TouchEndEventHandler);

	if (m_pxMediaView) {
		m_pxMediaView->Finished.RemoveListener<Preamble>(this, &Preamble::MediaFinishedEventHandler);
	}
	
	if (m_pxBackground) {
		delete m_pxBackground;
	}
}

void Preamble::Initialize() {
	if (m_sText.empty()) {
		SetCompletionState(eCompleted);
		return;
	}
}

void Preamble::OnDoLayout(const CIwSVec2& screensize) {
	int32 margin = GetScreenExtents() / 5;
	CIwVec2 pos(margin, margin);
	CIwVec2 size(screensize.x - 2 * margin, screensize.y - 2 * margin);
	m_pxMediaView->Show(pos, size);
}

void Preamble::OnUpdate(const FrameData& frame) {
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
	m_pxMediaView->Hide();
}

void Preamble::MediaFinishedEventHandler(const MediaView& sender, const int& args) {
	SetCompletionState(eCompleted);
	m_pxMediaView->Hide();
}

MediaView& Preamble::SelectMediaView(const std::string& file) {
	IW_CALLSTACK_SELF;

	if (m_xWeb.IsFileSupported(file)) {
		return m_xWeb;
	} 

	if (m_xVideo.IsFileSupported(file)) {
		return m_xVideo;
	}

	IwAssertMsg(MYAPP, false, ("Cannot itentify player for file '%s'; the file type is not mapped.", file.c_str()));
	return m_xNull;
}

