#ifndef __GAMEFOUNDATION_H__
#define __GAMEFOUNDATION_H__

#include <map>
#include <queue>
#include <list>

#include "Renderable.h"
#include "Sprite.h"
#include "Body.h"
#include "Buff.h"
#include "Star.h"
#include "World.h"
#include "ContactHandler.h"
#include "RayCaster.h"
#include "DustCounter.h"
#include "Curtain.h"

class GameFoundation : public Renderable {
public:
	enum BuffType {
		eBuffTypeMagnet,
		eBuffTypeShield,
		eBuffTypeShoot
	};
	
	typedef std::map<BuffType, int> BuffContainer;

	struct EnemyRef {
		std::string BodyId;
		float Distance;
	};
	typedef std::list<EnemyRef> EnemyRefs;

private:
	World m_xWorld;
	CIwFVec2 m_xWorldSize;
	RayCaster m_xRayCaster;
	ContactHandler m_xContactHandler;

	Curtain m_xShootBuffCurtain;
	uint32 m_uiShootBuffTimer;

	typedef std::map<std::string, Sprite*> SpriteMap;
	SpriteMap m_xSpriteMap;

	struct BodyInfo {
		std::string id;
		CIwFVec2 speed;
		CIwFVec2 position;
	};
	std::queue<BodyInfo> m_xBodyCreationQueue;

	Star* m_pxStar;
	DustCounter m_xDust;

	std::list<std::string> m_xMagnetSprites;
	std::list<std::string> m_xShootSprites;
	
public:
	GameFoundation(float dustrequirement, const CIwFVec2& worldsize);
	~GameFoundation();

	void Initialize();
	
	// sprite management
	void Add(Sprite* sprite);
	void Add(Body* body);
	
	void RegisterStar(Star* star);
	void UnregisterStar();
	
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

	// gameplay
	void ActivateMagnetBuff();
	void ActivateShieldBuff();
	void ActivateShootBuff();
	
	void BeginDustQueue();
	void AddDust(const CIwFVec2& pos, int amount);
	void CommitDustQueue(const CIwFVec2& pos);
	void RollbackDustQueue(const CIwFVec2& pos);

	float GetDustQueuedAmount();
	float GetDustQueuedPercent();

	float GetDustFillMax();
	float GetDustFillAmount();
	float GetDustFillPercent();
		
protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);

private:
	void CreatePointSplash(int amount, int bonusmultiplier, const CIwFVec2& position);

	void CreateSplashText(std::string text, const CIwFVec2& position, uint32 colour, Renderer::FontType font);
	void CreateBody(std::string id, const CIwFVec2& position, const CIwFVec2& speed);

	void UpdatePhysics(uint16 timestep);
	void ManageSpriteLifeCicles(const FrameData& frame);

	void EnqueueCreateBody(std::string id, const CIwFVec2& position, const CIwFVec2& speed);

	void UpdateMagnet(uint16 timestep);
	void UpdateShoot(uint16 timestep);
	
	bool CheckOutOfWorld(const CIwFVec2& pos);
	bool CheckOutOfUniverse(const CIwFVec2& pos);
	bool CheckOutOfBounds(const CIwFVec2& pos, float margin);

	int GetDustMultiplier(int queuedcount);

	void EmitBuff(const CIwFVec2& pos, const Body::BuffProbabilities& probs);
	
	void DustEventHandler(const Body& sender, const Star::DustEventArgs& args);
	void StarKilledEventHandler(const Star& sender, const int& args);
	void StarMagnetEventHandler(const Star& sender, const std::string& args);
	void BuffRequestedEventHandler(const Body& sender, const Body::EmitBuffArgs& args);
	void BuffCollectedEventHandler(const Buff& sender, const Buff::BuffArgs& args);
	void EffectRequestedEventHandler(const Body& sender, const Body::EffectArgs& args);

public:
	struct QuakeImpactArgs {
		float amplitude;
	};
	MulticastEvent<GameFoundation, QuakeImpactArgs> QuakeImpact;
	
	MulticastEvent<GameFoundation, Sprite> SpriteAdded;

	struct SpriteRemovedArgs {
		Sprite* sprite;
		bool outofbounds;
	};
	MulticastEvent<GameFoundation, SpriteRemovedArgs> SpriteRemoved;

	MulticastEvent<GameFoundation, BuffType> BuffCollected;
};

#endif
