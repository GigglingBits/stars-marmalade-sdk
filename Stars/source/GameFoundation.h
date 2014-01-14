#ifndef __GAMEFOUNDATION_H__
#define __GAMEFOUNDATION_H__

#include <map>
#include <queue>

#include "Renderable.h"
#include "Sprite.h"
#include "Body.h"
#include "Star.h"
#include "World.h"
#include "ContactHandler.h"
#include "RayCaster.h"
#include "DustCounter.h"

class GameFoundation : public Renderable {
private:
	World m_xWorld;
	CIwFVec2 m_xWorldSize;
	RayCaster m_xRayCaster;
	ContactHandler m_xContactHandler;

	typedef std::map<std::string, Sprite*> SpriteMap;
	SpriteMap m_xSpriteMap;

	struct SplashTextInfo {
		std::string text;
		CIwFVec2 position;
	};
	std::queue<SplashTextInfo> m_xSplashtextCreationQueue;

	struct BodyInfo {
		std::string id;
		CIwFVec2 speed;
		CIwFVec2 position;
	};
	std::queue<BodyInfo> m_xBodyCreationQueue;

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
	void CreateSplashNumber(long number, const CIwFVec2& position);
	void CreateSplashText(std::string text, const CIwFVec2& position);
	void CreateBody(std::string id, const CIwFVec2& position, const CIwFVec2& speed);

	// gameplay
	void EnqueueDust(const CIwFVec2& pos, int amount);
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
	void EnqueueCreateBody(std::string id, const CIwFVec2& position, const CIwFVec2& speed);

	bool CheckOutOfWorld(const CIwFVec2& pos);
	bool CheckOutOfUniverse(const CIwFVec2& pos);
	bool CheckOutOfBounds(const CIwFVec2& pos, float margin);

	void EmitBuff(const CIwFVec2& pos);
	
	void DustEventHandler(const Star& sender, const Star::DustEventArgs& args);
	void EffectRequestedEventHandler(const Body& sender, const Body::EffectArgs& args);

public:
	struct QuakeImpactArgs {
		float amplitude;
	};
	Event<GameFoundation, QuakeImpactArgs> QuakeImpact;
};

#endif
