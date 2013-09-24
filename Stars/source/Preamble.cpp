#include "Preamble.h"
#include "Debug.h"
#include "FactoryManager.h"
#include "MediaViewFactory.h"

Preamble::Preamble(const std::string& text, const std::string& textureid, const std::string& mediafile) : m_xTextPosition(0, 0, 0, 0), m_pxMediaView(NULL) {
	IW_CALLSTACK_SELF;
	IwAssertMsg(MYAPP, !text.empty() || !textureid.empty() || !mediafile.empty(), ("At least on of the 3 parguments must be non-empty."));

	m_sText = text;

	m_pxBackground = FactoryManager::GetTextureFactory().Create("preamble_bg");

	if (!textureid.empty()) {
		if (Texture* texture = FactoryManager::GetTextureFactory().Create(textureid)) {
			m_pxMediaView = MediaViewFactory::CreateViewForTexture(texture);
		}
		IwAssertMsg(MYAPP, m_pxMediaView, ("Error in assignment of texture media: %s", textureid.c_str()));

	} else if (!mediafile.empty()) {
		m_pxMediaView = MediaViewFactory::CreateViewForFile(mediafile);
		IwAssertMsg(MYAPP, m_pxMediaView, ("Error in assignment of media file: %s", textureid.c_str()));
	}
	
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
		delete m_pxMediaView;
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
	
	if (m_pxMediaView) {
		m_pxMediaView->Initialize();
	}
}

void Preamble::OnDoLayout(const CIwSVec2& screensize) {
	int32 margin = GetScreenExtents() / 5;

	if (m_pxMediaView) {
		// media view
		CIwVec2 pos(margin, margin);
		CIwVec2 size(screensize.x - 2 * margin, screensize.y - 2 * margin);
		m_pxMediaView->SetPosition(pos, size);
	
		// text
		m_xTextPosition.Make(margin, screensize.y - margin, screensize.x - (2 * margin), margin);
	} else {
		m_xTextPosition.Make(margin, margin, screensize.x - (2 * margin), screensize.y - 2 * margin);
	}
}

void Preamble::OnUpdate(const FrameData& frame) {
	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
    }
	if (m_pxMediaView) {
		m_pxMediaView->Update(frame);
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

	// media
	if (m_pxMediaView) {
		m_pxMediaView->Render(renderer, frame);
	}
	
	// text
	renderer.DrawText(m_sText, m_xTextPosition, Renderer::eFontTypeNormal, 0xffffffff);
}

void Preamble::TouchEndEventHandler(const InputManager& sender, const InputManager::TouchEventArgs& args) {
	SetCompletionState(eCompleted);

	if (m_pxMediaView) {
		delete m_pxMediaView;
		m_pxMediaView = NULL;
	}
}

void Preamble::MediaFinishedEventHandler(const MediaView& sender, const int& args) {
	SetCompletionState(eCompleted);

	if (m_pxMediaView) {
		delete m_pxMediaView;
		m_pxMediaView = NULL;
	}
}
