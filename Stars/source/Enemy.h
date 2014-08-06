#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "CompositeBody.h"
#include "ParticleSystem.h"

#include <set>

#define ENEMY_SLEDGE_BODY "sledge"
#define ENEMY_SLEDGE_CHILD "sledge"
#define ENEMY_SLEDGE_JOINT "sledge"

class Enemy : public CompositeBody {
private:
	ParticleSystem* m_pxParticles;
	bool m_bKnockedOut;
	bool m_bNeedDetachSledge;
	
	std::set<std::string> m_xSoftSpots;
	
public:
	Enemy(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef);

	virtual const char* GetTypeName();
	static const char* TypeName();
	
	void RegisterSoftsopt(const std::string& childid);
	void UnregisterSoftsopt(const std::string& childid);
	
	virtual void KnockOut();

private:
	void AttachSledge();
	void DetachSledge();
	
protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
	virtual void OnChildColliding(Body& child, Body& body);
};

#endif
