#ifndef _IMAGE2D_H
#define _IMAGE2D_H

#include <string>
#include "Iw2D.h"
#include "IwTexture.h"
#include "BaseBrick.h"

class Image2D : public BaseBrick {
private:
	CIwTexture* m_pxTexture;
	CIw2DImage* m_pxImage;

	CIwFVec2 m_xFocusPoint;
	CIwSVec2 m_xTexelFocusPoint;

public:
	Image2D();
	~Image2D();

	void SetImageFromTexture(CIwTexture* texture);
	void SetFocusPoint(const CIwFVec2& focuspoint);

	void Clear();

private:
	inline bool IsUploaded();
	inline float CalculateScalingFactor(const CIwSVec2& texturesize, const CIwSVec2& outputsize);
	inline CIwRect CalculateTextureRegion(const CIwSVec2& texturesize, const CIwSVec2& focuspoint, const CIwSVec2& outputsize);

	bool OnUpdate(bool first);
	void OnRender(const CIwSVec2& screensize, const CIwRect& itemregion);
};

#endif
