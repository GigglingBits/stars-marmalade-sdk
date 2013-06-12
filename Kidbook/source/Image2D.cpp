#include "Image2D.h"

#include "IwGx.h"
#include "IwDebug.h"

Image2D::Image2D() {
	m_pxTexture = NULL;
	m_pxImage = NULL;

	m_xFocusPoint = CIwSVec2(0, 0);
	m_xTexelFocusPoint = CIwSVec2(0, 0);
}

Image2D::~Image2D() {
	Clear();
}

void Image2D::SetImageFromTexture(CIwTexture* texture) {
	if (!texture) return;
	if (m_pxTexture) delete m_pxTexture;

	m_pxTexture = texture;
	m_pxTexture->SetMipMapping(false);

	if (m_xFocusPoint.x == 0 && m_xFocusPoint.y == 0) {
		// define the area that will always be visible (set to image center)
		m_xTexelFocusPoint.x = m_pxTexture->GetWidth() / 2;
		m_xTexelFocusPoint.y = m_pxTexture->GetHeight() / 2;
	} else {
		// define the area that will always be visible (set to image center)
		m_xTexelFocusPoint.x = (int16)(m_pxTexture->GetWidth() * m_xFocusPoint.x);
		m_xTexelFocusPoint.y = (int16)(m_pxTexture->GetHeight() * m_xFocusPoint.y);
	}
}

void Image2D::SetFocusPoint(const CIwFVec2& focuspoint) {
	m_xFocusPoint = focuspoint;
}

bool Image2D::IsUploaded() {
	return m_pxImage != NULL;
}

float Image2D::CalculateScalingFactor(const CIwSVec2& texturesize, const CIwSVec2& outputsize) {
	// Mission: Find the scaling factor for resizing the output size so 
	// that it can fit into the texture
	return std::max(
		FloatAbs((float)outputsize.x/(float)texturesize.x),
		FloatAbs((float)outputsize.y/(float)texturesize.y));
}

CIwRect Image2D::CalculateTextureRegion(const CIwSVec2& texturesize, const CIwSVec2& focuspoint, const CIwSVec2& outputsize) {	
	// Mission: Find the region within the texture that should be visible on 
	//          the on the screen (the output)

	// scale the output size down to fit inside the texture
	float scale = CalculateScalingFactor(texturesize, outputsize);
	int16 size_x = (int16)(outputsize.x / scale);
	int16 size_y = (int16)(outputsize.y / scale);

	// find region arount focus point
	int16 offset_x = focuspoint.x - (size_x / 2);
	int16 offset_y = focuspoint.y - (size_y / 2);

	// make sure that the region is actally on the texture
	if (offset_x < 0) {
		offset_x = 0;
	} else if (offset_x + size_x > texturesize.x) {
		offset_x = texturesize.x - size_x;
	}
	if (offset_y < 0) {
		offset_y = 0;
	} else if (offset_y + size_y > texturesize.y) {
		offset_y = texturesize.y - size_y;
	}

	return CIwRect(offset_x, offset_y, size_x, size_y);
}

bool Image2D::OnUpdate(bool first)  {
	if (!IsUploaded()) {
		if (!m_pxTexture) {
			return false;
		}
		m_pxImage = Iw2DCreateImage(m_pxTexture->GetImage());
		return true;
	}
	return false;
}

void Image2D::OnRender(const CIwSVec2& screensize, const CIwRect& itemregion) {
	if (IsUploaded()) {
		CIwSVec2 texturesize(m_pxImage->GetWidth(), m_pxImage->GetHeight());
		CIwRect textureregion = CalculateTextureRegion(texturesize, m_xTexelFocusPoint, CIwSVec2(itemregion.w, itemregion.h));

		Iw2DDrawImageRegion(m_pxImage,
			CIwFVec2(itemregion.x, itemregion.y),
			CIwFVec2(itemregion.w, itemregion.h),
			CIwFVec2(textureregion.x, textureregion.y),
			CIwFVec2(textureregion.w, textureregion.h));
	}
}

void Image2D::Clear() {
	if (m_pxTexture) {
		delete m_pxTexture;
		m_pxTexture = NULL;
	}

	if (m_pxImage) {
		delete m_pxImage;
		m_pxImage = NULL;
	}
}
