#include "LevelMenu.h"
#include "FactoryManager.h"
#include "Debug.h"
#include "SoundEngine.h"
#include "Configuration.h"

#define NO_BUTTON -1

LevelMenu::LevelMenu() : 
	Page() {
	m_pxBackground = FactoryManager::GetTextureFactory().Create("levelmenu_bg");
}

LevelMenu::~LevelMenu() {
	if (m_pxBackground) {
		delete m_pxBackground;
	}

	SoundEngine::StopMusicFile();
	
	IwGetUIView()->DestroyElements();
}

void LevelMenu::Initialize() {
	SoundEngine::PlayMusicFileLoop(Configuration::GetInstance().IntroSong);

	SetupControls();
}

void LevelMenu::SetupControls() {
	// start with blank view
	IwGetUIView()->DestroyElements();

	// load view from resources
	IwGetResManager()->LoadGroup("views.group");
    CIwUIElement* element = CIwUIElement::CreateFromResource("view");
	IwGetUIView()->AddElement(element);
	IwGetUIView()->AddElementToLayout(element);

	// add tiles
	AddTile("tile1", "image1");
	AddTile("tile2", "image2");
	AddTile("tile3", "image3");
	AddTile("tile4", "image4");
	AddTile("tile5", "image5");
	AddTile("tile6", "image6");
	AddTile("tile7", "image7");
	AddTile("tile8", "image8");
	AddTile("tile9", "image9");
}

void LevelMenu::AddTile(const std::string& id, const std::string& image) {
/*
	if (CIwUIElement* view = IwGetUIView()->GetChildNamed("view")) {
		if (CIwUILayout* layout = view->GetLayout()) {
			if (CIwUIElement* element = view->GetChildNamed("tile")) {
				element = element->Clone();
				element->SetName(id.c_str());
				layout->AddElement(element);
			} else {
				IwAssertMsg(MYAPP, false, ("Element not found"));
			}
		} else {
			IwAssertMsg(MYAPP, false, ("Layout not found"));
		}
	} else {
		IwAssertMsg(MYAPP, false, ("View not found"));
	}
*/

	//IwGetResManager()->LoadGroup("views.group");
	if (CIwUIElement* element = CIwUIElement::CreateFromResource("LevelButton")) {
		element->SetName(id.c_str());
		if (CIwUIElement* view = IwGetUIView()->GetChildNamed("view")) {
			view->AddChild(element);
			if (CIwUILayout* layout = view->GetLayout()) {
				layout->AddElement(element);
			} else {
				IwAssertMsg(MYAPP, false, ("Layout not found"));
			}
		} else {
			IwAssertMsg(MYAPP, false, ("View not found"));
		}
	} else {
		IwAssertMsg(MYAPP, false, ("Element not found"));
	}
}

void LevelMenu::OnUpdate(const FrameData& frame) {
	// update other buttons
	if (m_pxBackground) {
		m_pxBackground->Update(frame.GetRealDurationMs());
    }
}

void LevelMenu::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.SetViewport(m_xCamera.GetViewport());

    // background
    if (m_pxBackground) {
        VertexStreamScreen shape;        
        const CIwSVec2& screensize = frame.GetScreensize();
        shape.SetRect(CIwRect(0, 0, screensize.x, screensize.y));
        renderer.Draw(shape, *m_pxBackground);
    }
}

void LevelMenu::OnDoLayout(const CIwSVec2& screensize) {
}

