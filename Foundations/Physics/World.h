#ifndef __WORLD_H__
#define __WORLD_H__

#include "Box2D.h"
#include "ContactListener.h"

class World {
private:
    static b2World* s_pxWorld;
	static b2Body* s_pxAnchorBody;

	static int s_iReferenceCounter;

	static b2Vec2 s_xGravity;

public:
	World();
	virtual ~World();

	b2World& GetWorld();

	void SetContactListener(ContactListener& listener);
	void RemoveContactListener();

	b2Body& GetAnchorBody();

	static void SetDefaultGravity(float x, float y);

private:
	static void Create();
	static void Destroy();
};

#endif
