#ifndef _BASEBRICK_H_ 
#define _BASEBRICK_H_ 

#include "IwGxTypes.h"
#include "IwGeomSVec2.h"

class BaseBrick {
public:
	bool Update(bool first);
	void Render(const CIwSVec2& screensize, const CIwRect& itemregion);

protected:
	virtual bool OnUpdate(bool first) = 0;
	virtual void OnRender(const CIwSVec2& screensize, const CIwRect& itemregion) = 0;

protected:
	static float FloatAbs(float f);
	static float FloatDivide(float x, float y);

	static bool IsOnScreen(const CIwSVec2& screensize, const CIwRect& itemregion);

	static void ConvertToScreenspaceCoords(const CIwRect& outputregion, CIwSVec2 coords[4]);
	static void ConvertToUvCoords(const CIwSVec2& texturesize, const CIwRect& textureregion, CIwFVec2 uvs[4]);
};

#endif
