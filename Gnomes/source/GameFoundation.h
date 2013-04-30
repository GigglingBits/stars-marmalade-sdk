#ifndef __GAMEFOUNDATION_H__
#define __GAMEFOUNDATION_H__

#include <map>
#include <queue>

#include "Event.h"

#include "Renderable.h"
#include "Sprite.h"
#include "Body.h"
#include "Hero.h"
#include "World.h"
#include "ContactListener.h"
#include "RayCaster.h"
#include "LevelCompletionInfo.h"

class GameFoundation : public Renderable {
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

	LevelCompletionInfo m_xCompletionInfo;
	int m_iHeroCount;

public:
	GameFoundation(const CIwFVec2& worldsize);
	~GameFoundation();

	// sprite management
	void Add(Sprite* sprite);
	void Add(Body* body);

	Sprite* FindSprite(const std::string& id);

	template <class TSprite> 
	TSprite* FindSprite(const std::string& id) {
		return dynamic_cast<TSprite*>(FindSprite(id));
	}

	std::map<std::string, Sprite*>& GetSpriteMap();	

	// interaction
	bool RayHitTest(CIwFVec2 raystart, CIwFVec2 rayend);

	// game state
	CIwFVec2 GetGravity();
	bool IsCompleted();
	void GetCompletionInfo(LevelCompletionInfo& info);

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);

private:
	void UpdatePhysics(uint16 timestep);
	void ManageSpriteLifeCicles(const FrameData& frame);
	void EnqueueCreateSplashText(std::string text, const CIwFVec2& position);

	void Kill(Hero* hero);
	void Rescue(Hero* hero);

	bool CheckOutOfBounds(const CIwFVec2& pos);

	void AddSplashText(std::string text, const CIwFVec2& position);

	void Collide(Body& body1, Body& body2, bool issensorcollision, const CIwFVec2 collisionpoint, float approachvelocity);
	void CollisionEventHandler(const ContactListener& sender, const ContactListener::CollisionEventArgs& args);

public:
	struct SoundEventArgs {
		CIwFVec2 position;
		float impulse;
		std::string soundgroup;
	};
	Event<GameFoundation, SoundEventArgs> SoundEffectEvent;

	enum HeroChangeTypes {
		eHeroChangeBorn,
		eHeroChangeKilled,
		eHeroChangeRescued
	};

	struct HeroChangedEventArgs {
		std::string id;
		HeroChangeTypes changetype;
	};
	Event<GameFoundation, HeroChangedEventArgs> HeroChangedEvent;
};

#endif
