#ifndef __GAMEFOUNDATION_H__
#define __GAMEFOUNDATION_H__

#include <map>
#include <queue>

#include "Renderable.h"
#include "Sprite.h"
#include "Body.h"
#include "Star.h"
#include "World.h"
#include "EventTimer.h"
#include "ContactListener.h"
#include "RayCaster.h"

class GameFoundation : public Renderable {
public:
	struct CompletionInfo {
		bool IsCleared;
		CompletionInfo() {
			IsCleared = false;
		}
	};

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

	typedef struct {
		std::string Body;
		float YPos;
	} BodySpec;
	EventTimer<BodySpec> m_xBodyTimer;

	typedef std::map<std::string, Sprite*> SpriteMap;
	SpriteMap m_xSpriteMap;

	std::queue<SplashTextInfo> m_xSplashtextCreationQueue;

	CompletionInfo m_xCompletionInfo;
	Star* m_pxStar;

public:
	GameFoundation(const CIwFVec2& worldsize);
	~GameFoundation();

	// sprite management
	void Add(Sprite* sprite);
	void Add(Body* body);

	void Add(uint16 delay, const std::string& body, float ypos);

	Star* GetStar();
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
	const CompletionInfo& GetCompletionInfo();
	float GetCompletionDegree();

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);

private:
	void UpdatePhysics(uint16 timestep);
	void ManageSpriteLifeCicles(const FrameData& frame);
	void EnqueueCreateSplashText(std::string text, const CIwFVec2& position);

	bool CheckOutOfBounds(const CIwFVec2& pos);

	void AddSplashText(std::string text, const CIwFVec2& position);

	void Collide(Body& body1, Body& body2, bool issensorcollision, const CIwFVec2 collisionpoint, float approachvelocity);
	void CollisionEventHandler(const ContactListener& sender, const ContactListener::CollisionEventArgs& args);

	void BodyTimerEventHandler(const EventTimer<BodySpec>& sender, const BodySpec& args);
	void BodyTimerClearedEventHandler(const EventTimer<BodySpec>& sender, const int& dummy);
};

#endif
