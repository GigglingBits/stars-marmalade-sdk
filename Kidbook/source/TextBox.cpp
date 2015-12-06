#include "Iw2D.h"
#include "TextBox.h"

TextBox::TextBox() {
	m_bShow = false;
	m_bAnimating = false;
	m_iAlpha = 0;
}

TextBox::~TextBox() {
}

void TextBox::SetText(std::string text) {
	m_sText	= text;
}

void TextBox::SetVisible(bool visible) {
	if (m_bShow == visible) {
		return;
	}
	m_bShow = visible;
}

bool TextBox::IsVisible() {
	return m_bShow;
}

bool TextBox::OnUpdate(bool first) {
	if (!first) {
		return false;
	}

	int targetalpha = m_bShow ? 210 : 0;
	m_bAnimating = m_iAlpha != targetalpha;
	if (m_bAnimating) {
		const int stepsize = 7;
		if (std::abs(targetalpha - m_iAlpha) < stepsize) {
			m_iAlpha = targetalpha;
		} else {
			m_iAlpha = m_iAlpha + ((targetalpha - m_iAlpha) / stepsize);
		}
	}

	return false;
}

void TextBox::OnRender(const CIwSVec2& screensize, const CIwRect& itemregion) {
	if (IwGxFontGetFont() == NULL) {
		return;
	}

	const uint8 gray = 240;

	if (m_bAnimating || m_bShow) {
		// background
		CIwColour oldcol = Iw2DGetColour();
		Iw2DSetColour(m_iAlpha << 24 | gray << 16 | gray << 8 | gray << 0);
		Iw2DFillRect(
			CIwFVec2(itemregion.x, itemregion.y),
			CIwFVec2(itemregion.w, itemregion.h));
		Iw2DSetColour(oldcol);
	}

	if (m_bShow) {
		Iw2DFinishDrawing();
		// text
		const int margin = 20;
		IwGxFontSetCol(0xff000000);
		IwGxFontSetRect(CIwRect(
			itemregion.x + margin, 
			itemregion.y, 
			itemregion.w - margin, 
			itemregion.h));
		IwGxFontDrawText(m_sText.c_str());
	}
}
