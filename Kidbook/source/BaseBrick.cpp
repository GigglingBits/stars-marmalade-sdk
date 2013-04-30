#include "BaseBrick.h"

bool BaseBrick::Update(bool first) {
	return OnUpdate(first);
}

void BaseBrick::Render(const CIwSVec2& screensize, const CIwRect& itemregion) {
	if (IsOnScreen(screensize, itemregion)) {
		OnRender(screensize, itemregion);
	}
}

bool BaseBrick::IsOnScreen(const CIwSVec2& screensize, const CIwRect& itemregion) {
	return 
		itemregion.x + itemregion.w > 0 &&
		itemregion.x < screensize.x &&
		itemregion.y + itemregion.h > 0 &&
		itemregion.y < screensize.y;
}

float BaseBrick::FloatAbs(float f) {
	return f < 0 ? -1 * f : f;
}

float BaseBrick::FloatDivide(float x, float y) {
	return x / y;
}

void BaseBrick::ConvertToScreenspaceCoords(const CIwRect& outputregion, CIwSVec2 coords[4]) {
	// Mission: Convert the display geometry to screenspace coordinates
	//          (required by the rendering engine)
    coords[0].x = outputregion.x, coords[0].y = outputregion.y;
    coords[1].x = outputregion.x, coords[1].y = outputregion.y + outputregion.h;
    coords[2].x = outputregion.x + outputregion.w, coords[2].y = outputregion.y + outputregion.h;
    coords[3].x = outputregion.x + outputregion.w, coords[3].y = outputregion.y;
}

void BaseBrick::ConvertToUvCoords(const CIwSVec2& texturesize, const CIwRect& textureregion, CIwFVec2 uvs[4]) {
	// Mission: Convert the texture geometry from screen coordinates 
	//          to UV coordinates (required for UV mapping in the rendering engine)
	float topmargin = FloatDivide((float)textureregion.y, (float)texturesize.y);
	float leftmargin = FloatDivide((float)textureregion.x, (float)texturesize.x);
	float rightmargin = FloatDivide((float)(texturesize.x - textureregion.w - textureregion.x), (float)texturesize.x);
	float bottommargin = FloatDivide((float)(texturesize.y - textureregion.h - textureregion.y), (float)texturesize.y);

	uvs[0].x = leftmargin, uvs[0].y = topmargin;  
	uvs[1].x = leftmargin, uvs[1].y = 1.0f - bottommargin;  
	uvs[2].x = 1.0f - rightmargin, uvs[2].y = 1.0f - bottommargin;  
	uvs[3].x = 1.0f - rightmargin, uvs[3].y = topmargin;
}
