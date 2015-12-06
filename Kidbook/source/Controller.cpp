#include "Controller.h"

#include "s3e.h"
#include "IwDebug.h"
#include "Configuration.h"

Controller::Controller() {
	m_xOffset = CIwSVec2(0, 0);
	m_bIsTouchMoving = false;
	m_bIsAnimating = 0;

	m_xPageToolbar.AddButton(BTN_ID_SOUND);
	if (Configuration::GetInstance().EnableSpeech) {
		m_xPageToolbar.AddButton(BTN_ID_SPEECH);
	}
	if (Configuration::GetInstance().EnableText) {
		m_xPageToolbar.AddButton(BTN_ID_TEXT);
	}
	if (Configuration::GetInstance().ExitButton) {
		m_xAppToolbar.AddButton(BTN_ID_EXIT);
	}
	if (Configuration::GetInstance().HelpButton) {
		m_xAppToolbar.AddButton(BTN_ID_HELP);
	}

	m_pxSpectator = NULL;
	m_pxWelcome = NULL;
}

Controller::~Controller() {
	if (m_pxSpectator) {
		delete m_pxSpectator;
	}
	if (m_pxWelcome) {
		delete m_pxWelcome;
	}
}

void Controller::Initialize() {
	m_xPageMap.SetLanguage(Configuration::GetInstance().Language);
	m_xPageMap.SetLocale(Configuration::GetInstance().Locale);
	m_xPageMap.Load(Configuration::GetInstance().PageMapFile);
	if (!m_pxSpectator) {
		m_pxSpectator = new Spectator(m_xPageMap);
	}
	UpdateToolbar();
}

void Controller::TouchMove(const CIwSVec2& touch, const CIwSVec2& move) {
	if (IsHelpVisible()) {
		// moves are not processes when 
		// help is being shown
		return;
	}

	m_xOffset = move - touch;
	m_bIsTouchMoving = true;
}

void Controller::Touch(const CIwSVec2& touch) {
	switch(m_xAppToolbar.TouchButton(touch)) {
	case BTN_ID_EXIT:
		s3eDeviceRequestQuit();
		break;

	case BTN_ID_HELP:
		BtnHelpPressed();
		break;
	}

	if (IsHelpVisible()) {
		// page buttons are not processed when 
		// help is being shown
		return;
	}

	switch(m_xPageToolbar.TouchButton(touch)) {
	case BTN_ID_SPEECH:
		BtnVoicePressed();
		break;

	case BTN_ID_TEXT:
		BtnTextPressed();
		break;

	case BTN_ID_SOUND:
		BtnSoundPressed();
		break;
	}
}

void Controller::Shake() {
	Page* page;
	if ((page = m_pxSpectator->GetPage(eCenter))) {
		PlaySound(page->m_sSoundName, false);
	}
}

bool Controller::IsHelpVisible() {
	return m_pxWelcome != NULL;
}

void Controller::BtnTextPressed() {
	bool shown = m_pxSpectator->GetPage(eCenter)->IsTextShown();
	ShowText(!shown);
}

void Controller::BtnSoundPressed() {
	Page* page;
	if ((page = m_pxSpectator->GetPage(eCenter))) {		
		PlaySound(page->m_sSoundName, true);
	}
}

void Controller::BtnVoicePressed() {
	Page* page;
	if ((page = m_pxSpectator->GetPage(eCenter))) {		
		PlaySound(page->m_sSpeechName, true);
	}
}

void Controller::BtnHelpPressed() {
	if (m_pxWelcome) {
		delete m_pxWelcome;
		m_pxWelcome = NULL;
	} else {
		m_pxWelcome = new Welcome();
	}
}

bool Controller::OnUpdate(bool first) {
	if (first) {
		if (m_bIsTouchMoving) {
			// still touch-moving; make ready for next call
			m_bIsTouchMoving = false;
			// reset animation
			StopAnimation();
		} else {
			// no longer touch-moving
			if (IsAnimating()) {
				// animation is started
				AnimateOffset(m_xOffset);
				// detect end of animation
				if (!HasOffset()) {
					StopAnimation();
				}
			} else {
				// detect start new animation
				if (HasOffset()) {
					// detect page changes
					DoPageChange();
				}
			}
		}
	}
	if (m_xAppToolbar.Update(first)) {
		return true;
	}
	if (m_xPageToolbar.Update(first)) {
		return true;
	}
	if (m_pxWelcome && m_pxWelcome->Update(first)) {
		return true;
	}
	return m_pxSpectator->Update(first);
}

void Controller::OnRender(const CIwSVec2& screensize, const CIwRect& itemregion) {
	// render all images (the spectator covers 3x3 times the screensize)
	CIwRect spectatorregion(
		m_xOffset.x - itemregion.w,
		m_xOffset.y - itemregion.h,
		itemregion.w * 3, 
		itemregion.h * 3);
	m_pxSpectator->Render(screensize, spectatorregion);

	// render app toolbar
	int height = std::min(std::min(screensize.x, screensize.y) / 6, 150);
	int width = m_xAppToolbar.GetButtonCount() * height;
	m_xAppToolbar.Render(
		screensize, 
		CIwRect(
			itemregion.x + itemregion.w - width, 
			itemregion.y,
			width, height));

	// render page toolbar (if help is not shown) 
	if (!IsHelpVisible()) {
		width = m_xPageToolbar.GetButtonCount() * height;
		m_xPageToolbar.Render(
			screensize, 
			CIwRect(
				itemregion.x + itemregion.w - width, 
				itemregion.y + itemregion.h - height, 
				width, height));
	}

	// welcome screen
	if (m_pxWelcome) {
		CIwRect safearea = itemregion;
		safearea.w -= 2 * height;
		m_pxWelcome->Render(screensize, safearea);
	}
}

bool Controller::HasOffset() {
	return m_xOffset.x != 0 || m_xOffset.y != 0;
}

void Controller::DoPageChange() {
	const int threshold = 6;

	int32 width = Iw2DGetSurfaceWidth();
	int32 height = Iw2DGetSurfaceHeight();

	DIR_TYPE dir = DIR_CENTER;

	// find direction
	if (m_xOffset.x > (width/threshold)) {
		dir |= DIR_LEFT;
	} else if (m_xOffset.x < (-width/threshold)) {
		dir |= DIR_RIGHT;
	}
	
	if (m_xOffset.y > (height/threshold)) {
		dir |= DIR_UP;
	} else if (m_xOffset.y < (-height/threshold)) {
		dir |= DIR_DOWN;
	}
	
	// do the move
	Move((Direction)dir);
}

void Controller::Move(Direction dir) {
	if (IsHelpVisible()) {
		// page navigation is suppressed when 
		// help is being shown
		IwAssert(MYAPP, m_pxWelcome);
		if (DIR_IS_UP(dir) || DIR_IS_RIGHT(dir)) {
			m_pxWelcome->ScrollUp();
		} else if (DIR_IS_DOWN(dir) || DIR_IS_LEFT(dir)) {
			m_pxWelcome->ScrollDown();
		}
		return;
	}

	if (dir != eCenter) {
		int32 width = Iw2DGetSurfaceWidth();
		int32 height = Iw2DGetSurfaceHeight();

		if (DIR_IS_UP(dir)) {
			m_xOffset.y = std::max(m_xOffset.y - height, -height);
		}
		if (DIR_IS_DOWN(dir)) {
			m_xOffset.y = std::min(m_xOffset.y + height, height);
		}
		if (DIR_IS_RIGHT(dir)) {
			m_xOffset.x = std::min(m_xOffset.x + width, width);
		}
		if (DIR_IS_LEFT(dir)) {
			m_xOffset.x = std::max(m_xOffset.x - width, -width);
		}

		ShowText(false);
		m_pxSpectator->Move(dir);
		UpdateToolbar();
	}
	StartAnimation();
}

void Controller::UpdateToolbar() {
	Page* page = m_pxSpectator->GetPage(eCenter);
	if (!page) return;
	
	m_xPageToolbar.EnableButton(
		BTN_ID_SOUND, 
		!page->m_sSoundName.empty());
	
	m_xPageToolbar.EnableButton(
		BTN_ID_SPEECH, 
		!page->m_sSpeechName.empty());
	
	m_xPageToolbar.EnableButton(
		BTN_ID_TEXT, 
		!page->m_sPageName.empty());
}

void Controller::StartAnimation() {
	m_bIsAnimating = true;
}

void Controller::StopAnimation() {
	m_bIsAnimating = false;
}

bool Controller::IsAnimating() {
	return m_bIsAnimating;	
}

void Controller::AnimateOffset(CIwSVec2& offset) {
	static double factor = Configuration::GetInstance().AnimationConstant;
	offset.x = (int16)(factor*(float)offset.x);
	offset.y = (int16)(factor*(float)offset.y);
}

void Controller::PlaySound(std::string soundfile, bool interrupt) {
	if (s3eAudioIsPlaying() && !interrupt)  {
		return;
	}
	if (!soundfile.empty()) {
		if (S3E_RESULT_SUCCESS != s3eAudioPlay(soundfile.c_str(), 1)) {
			IwAssertMsg(MYAPP, false, ("failed to play %s because: %s", soundfile.c_str(), s3eAudioGetErrorString()));
		}
	}
}

void Controller::ShowText(bool show) {
	m_pxSpectator->GetPage(eCenter)->SetShowText(show);
}
