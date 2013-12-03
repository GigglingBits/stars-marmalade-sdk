#ifndef __PARTICLEEFFECT_H__
#define __PARTICLEEFFECT_H__

#include "LocalEffect.h"
#include "ParticleSystem.h"

class ParticleEffect : public LocalEffect {
private:
	ParticleSystem* m_pxSystem;
	
public:
	ParticleEffect(const std::string& id, const ShapeTemplate& shapedef, const TextureTemplate& texturedef);
	virtual ~ParticleEffect();

	virtual const char* GetTypeName();
	static const char* TypeName();

	void SetText(std::string text);

	virtual bool CanDispose();

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
