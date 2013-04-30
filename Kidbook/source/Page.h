#ifndef _PAGE_H_ 
#define _PAGE_H_ 

#include <string>

#include "JpegFile.h"
#include "Image2D.h"
#include "TextBox.h"

#include "BaseBrick.h"

class Page : public BaseBrick {
	enum State {
		eDecativated,
		eLoadingImage,
		eActive
	};

public:
	std::string m_sPageId;
	std::string m_sPageName;
	std::string m_sSoundName;
	std::string m_sSpeechName;

private:
	State m_eCurrentState;

	TextBox m_xTextBox;

	JpegFile m_xThumbnailFile;
	Image2D m_xThumbnail;

	JpegFile m_xImageFile;
	Image2D m_xImage;

	JpegDecompressor* m_pxImageDecompressor;

public:
	Page();
	~Page();

	bool IsActive();
	void SetActive(bool active);

	bool IsTextShown();
	void SetShowText(bool show);

	void SetFocusPoint(const CIwFVec2& focuspoint);
	void SetImage(const std::string& filename);
	void SetThumbnail(const std::string& filename);

private:
	const CIwRect CalculateTextPosition(const CIwSVec2& screensize, const CIwRect& imageregion);
	bool CreateTexture(JpegDecompressor& decomp, Image2D& image);

	bool OnUpdate(bool first);
	void OnRender(const CIwSVec2& screensize, const CIwRect& itemregion);
};

#endif
