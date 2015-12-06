#include <string>

#include "IwGxPrint.h"
#include "IwTexture.h"
#include "IwJPEG.h"

#include "Page.h"

#include "Configuration.h"

Page::Page() {
	m_eCurrentState = eDecativated;
	m_pxImageDecompressor = NULL;
}

Page::~Page() {
	if (m_pxImageDecompressor) {
		delete m_pxImageDecompressor;
	}
}

bool Page::IsActive() {
	return 	m_eCurrentState != eDecativated;
}

void Page::SetActive(bool active) {
	if (IsActive() == active) {
		return;
	}
	if (active) {
		IwAssertMsg(MYAPP, !m_xImageFile.GetFileName().empty(), ("image name empty for page id: %s", m_sPageId.c_str()));
		m_eCurrentState = eLoadingImage;
	} else {
		if (m_pxImageDecompressor) {
			delete m_pxImageDecompressor;
			m_pxImageDecompressor = NULL;
		}
		m_xImage.Clear();
		IwTrace(MYAPP, ("deleted image for: %s", m_xImageFile.GetFileName().c_str()));
		m_eCurrentState = eDecativated;
	}
}

bool Page::IsTextShown() {
	return m_xTextBox.IsVisible();
}

void Page::SetShowText(bool show) {
	if (show) {
		m_xTextBox.SetText(m_sPageName);
	}
	m_xTextBox.SetVisible(show);
}

void Page::SetFocusPoint(const CIwFVec2& focuspoint) {
	m_xImage.SetFocusPoint(focuspoint);
	m_xThumbnail.SetFocusPoint(focuspoint);
}

void Page::SetImage(const std::string& filename) {
	m_xImageFile.LoadFile(filename);
}

void Page::SetThumbnail(const std::string& filename) {
	m_xThumbnailFile.LoadFile(filename);
	JpegDecompressor* pDecomp = m_xThumbnailFile.CreateDecompressor();
	if (pDecomp && CreateTexture(*pDecomp, m_xThumbnail)) {
		m_xThumbnail.Update(false); // make sure it becomes visible from the start
		IwTrace(MYAPP, ("created thumbnail for: %s", m_xThumbnailFile.GetFileName().c_str()));
	} else {
		IwAssertMsg(MYAPP, false, ("failed to create texture for thumbnail: %s", m_xThumbnailFile.GetFileName().c_str()));
	}
	delete pDecomp;
}

const CIwRect Page::CalculateTextPosition(const CIwSVec2& screensize, const CIwRect& imageregion) {
	int height = std::min(std::min(screensize.x, screensize.y) / 6, 150);
	return CIwRect(
			imageregion.x, 
			imageregion.y + imageregion.h - height, 
			imageregion.w, height);
}

bool Page::CreateTexture(JpegDecompressor& decomp, Image2D& image) {
	// ensure that all steps are completed
	while (decomp.Step()) {
	}

	// create and transfer texture
	CIwTexture* pTexture = decomp.GetTexture();
	if (pTexture != NULL) {
		image.SetImageFromTexture(pTexture);
		return true;
	}
	return false;
}

bool Page::OnUpdate(bool first) {
	// step 1: load image
	if (m_eCurrentState == eLoadingImage) {
		if (!m_pxImageDecompressor) {
			m_pxImageDecompressor = m_xImageFile.CreateDecompressor();
		}

		if (m_pxImageDecompressor->Step()) {
			return true;
		}

		// step 2: create texture when done decompressing
		if (CreateTexture(*m_pxImageDecompressor, m_xImage)) {
			IwTrace(MYAPP, ("created image for: %s", m_xImageFile.GetFileName().c_str()));
		} else {
			IwAssertMsg(MYAPP, false, ("failed to create texture for image: %s, ", m_xImageFile.GetFileName().c_str()));
		}
		delete m_pxImageDecompressor;
		m_pxImageDecompressor = NULL;

		m_eCurrentState = eActive;
		return true;
	}

	// step 3: care about other stuff
	IwAssertMsg(MYAPP, m_eCurrentState == eActive, ("Invalid state"));
	if (m_xImage.Update(first)) return true;
	if (m_xThumbnail.Update(first)) return true;
	if (m_xTextBox.Update(first)) return true;

	return false;
}

void Page::OnRender(const CIwSVec2& screensize, const CIwRect& itemregion) {
	// shrink image to create a frame
	CIwRect imageregion(itemregion);
	static int margin = std::min<int>(screensize.x, screensize.y) / 150;
	imageregion.x += margin, imageregion.y += margin;
	imageregion.w -= 2 * margin, imageregion.h -= 2 * margin;

	// render
	if (m_eCurrentState == eActive) {
		m_xImage.Render(screensize, imageregion);
		m_xTextBox.Render(screensize, CalculateTextPosition(screensize, imageregion));
	} else {
		m_xThumbnail.Render(screensize, imageregion);
	}

	// write name and ID
	if (Configuration::GetInstance().ShowStats) {
		std::string s;
		s = "Id:     " + m_sPageId;
		IwGxPrintString(imageregion.x + 20, imageregion.y + imageregion.h - 100, s.c_str(), false); 
		s = "Name:   " + m_sPageName;
		IwGxPrintString(imageregion.x + 20, imageregion.y + imageregion.h - 80, s.c_str(), false); 
		s = "Image:  " + m_xImageFile.GetFileName();
		IwGxPrintString(imageregion.x + 20, imageregion.y + imageregion.h - 60, s.c_str(), false); 
		s = "Sound:  " + m_sSoundName;
		IwGxPrintString(imageregion.x + 20, imageregion.y + imageregion.h - 40, s.c_str(), false); 
		s = "Speech: " + m_sSpeechName;
		IwGxPrintString(imageregion.x + 20, imageregion.y + imageregion.h - 20, s.c_str(), false); 
	}
}
