#ifndef __LOCALEFFECT_H__
#define __LOCALEFFECT_H__

#include "Sprite.h"
#include "ShapeTemplate.h"

class LocalEffect : public Sprite {
private:
	uint16 m_iRemaingTimeMS;
	bool m_bCanDispose;
	CIwFVec2 m_xVelocity;

public:
	LocalEffect(const std::string& id, const ShapeTemplate& shapedef, const TextureTemplate& texturedef);

	virtual const char* GetTypeName();
	static const char* TypeName();

	void SetFadeTime(uint16 ms);
	void SetVelocity(const CIwFVec2& v);

	virtual bool CanDispose();

protected:
	virtual void OnUpdate(const FrameData& frame);
};

#endif
