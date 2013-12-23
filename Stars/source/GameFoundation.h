#ifndef __GAMEFOUNDATION_H__
#define __GAMEFOUNDATION_H__

#include <map>
#include <queue>

#include "Renderable.h"
#include "Sprite.h"
#include "Body.h"
#include "Star.h"
#include "World.h"
#include "ContactListener.h"
#include "RayCaster.h"
#include "DustCounter.h"

class GameFoundation : public Renderable {
private:
	struct SplashTextInfo {
		std::string text;
		CIwFVec2 position;
	};

private:
	World m_xWorld;
	CIwFVec2 m_xWorldSize;
	RayCaster m_xRayCaster;
	ContactListener m_xContactListener;

	typedef std::map<std::string, Sprite*> SpriteMap;
	SpriteMap m_xSpriteMap;

	std::queue<SplashTextInfo> m_xSplashtextCreationQueue;

	Star* m_pxStar;

	DustCounter m_xDust;
	
public:
	GameFoundation(float dustrequirement, const CIwFVec2& worldsize);
	~GameFoundation();

	void Initialize();
	
	// sprite management
	void Add(Sprite* sprite);
	void Add(Body* body);

	Star* GetStar();
	Sprite* FindSprite(const std::string& id);

	template <class TSprite> 
	TSprite* FindSprite(const std::string& id) {
		return dynamic_cast<TSprite*>(FindSprite(id));
	}

	std::map<std::string, Sprite*>& GetSpriteMap();	

	// simulation
	bool IsGameGoing();
	
	// interaction
	bool RayHitTest(CIwFVec2 raystart, CIwFVec2 rayend);
	bool StarHitTest(CIwFVec2 position);

	// managed effects
	void AddSplashNumber(long number, const CIwFVec2& position);
	void AddSplashText(std::string text, const CIwFVec2& position);

	// gameplay
	void QueueDust(const CIwFVec2& pos, int amount);
	void CommitDust(const CIwFVec2& pos);
	void CancelDust(const CIwFVec2& pos);

	float GetDustQueuedAmount();
	float GetDustQueuedPercent();

	float GetDustFillAmount();
	float GetDustFillPercent();
	
protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);

private:
	void UpdatePhysics(uint16 timestep);
	void ManageSpriteLifeCicles(const FrameData& frame);
	void EnqueueCreateSplashText(std::string text, const CIwFVec2& position);

	bool CheckOutOfBounds(const CIwFVec2& pos);

	void Collide(Body& body1, Body& body2, bool issensorcollision, const CIwFVec2 collisionpoint, float approachvelocity);
	void CollisionEventHandler(const ContactListener& sender, const ContactListener::CollisionEventArgs& args);

public:
	struct QuakeImpactArgs {
		float amplitude;
	};
	Event<GameFoundation, QuakeImpactArgs> QuakeImpact;
};

#endif
