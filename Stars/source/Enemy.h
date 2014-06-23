#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "CompositeBody.h"
#include "ParticleSystem.h"

#define ENEMY_SLEDGE_BODY "sledge"
#define ENEMY_SLEDGE_CHILD "sledge"
#define ENEMY_SLEDGE_JOINT "sledge"

class Enemy : public CompositeBody {
private:
	ParticleSystem* m_pxParticles;
	bool m_bKnockedOut;
	bool m_bNeedDetachSledge;
	
public:
	Enemy(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);

	virtual const char* GetTypeName();
	static const char* TypeName();
	
private:
	void AttachSledge();
	void DetachSledge();
	
protected:
	virtual void KnockOut();
	
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnColliding(Body& body);
	virtual void OnChildColliding(Body& child, Body& body);
};

#endif
