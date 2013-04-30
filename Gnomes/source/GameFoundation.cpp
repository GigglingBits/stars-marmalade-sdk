#include "GameFoundation.h"
#include "SplashText.h"
#include "Target.h"
#include "Monster.h"
#include "FactoryManager.h"
#include "SoundEngine.h"

GameFoundation::GameFoundation(const CIwFVec2& worldsize) 
	: m_xWorldSize(worldsize), m_xRayCaster(m_xWorld) {

	m_iHeroCount = 0;

	m_xContactListener.CollisionEvent.AddListener<GameFoundation>(this, &GameFoundation::CollisionEventHandler);
	m_xWorld.SetContactListener(&m_xContactListener);
}

GameFoundation::~GameFoundation() {
	SpriteMap::iterator it;
	for (it = m_xSpriteMap.begin(); it != m_xSpriteMap.end(); ++it) {
		Sprite* sprite = it->second;
		delete sprite;
	}

	m_xWorld.SetContactListener(NULL);
	m_xContactListener.CollisionEvent.RemoveListener<GameFoundation>(this, &GameFoundation::CollisionEventHandler);
}

bool GameFoundation::IsCompleted() {
	return m_iHeroCount == 0;
}

void GameFoundation::GetCompletionInfo(LevelCompletionInfo& info) {
	info = m_xCompletionInfo;
}

std::map<std::string, Sprite*>& GameFoundation::GetSpriteMap() {
	return m_xSpriteMap;
}

Sprite* GameFoundation::FindSprite(const std::string& id) {
	SpriteMap::iterator it = m_xSpriteMap.find(id);
	if (it == m_xSpriteMap.end()) {
		return NULL;
	}
	return it->second;
}

void GameFoundation::Add(Body* body) {
	// indexing of hero
	if (body->GetTypeName() == Hero::TypeName()) {
		m_iHeroCount++;

		HeroChangedEventArgs args;
		args.id = body->GetId();
		args.changetype = eHeroChangeBorn;
		HeroChangedEvent.Invoke(*this, args);
	}

	body->SetGameFoundation(*this);

	Add((Sprite*)body);
}

void GameFoundation::Add(Sprite* sprite) {
	// make sure it has an id
	std::string id = sprite->GetId();
	IwAssertMsg(MYAPP, !id.empty(), ("A body could not be added to the game because no id was defined."));

	// register the body
	m_xSpriteMap[id] = sprite;
}

void GameFoundation::Kill(Hero* hero) {
	if (!hero) {
		return;
	}
	
	hero->GetHealthManager().Kill();


	m_xCompletionInfo.SetHeroState(
		hero->GetId(), false, 
		(float)hero->GetHealthManager().GetHealthValue());

	// notify
	HeroChangedEventArgs args;
	args.id = hero->GetId();
	args.changetype = eHeroChangeKilled;
	HeroChangedEvent.Invoke(*this, args);
}

void GameFoundation::Rescue(Hero* hero) {
	if (!hero) {
		return;
	}
	hero->GetHealthManager().Kill();
	m_xCompletionInfo.SetHeroState(
		hero->GetId(), true, 
		(float)hero->GetHealthManager().GetHealthValue());

	// notify
	HeroChangedEventArgs args;
	args.id = hero->GetId();
	args.changetype = eHeroChangeRescued;
	HeroChangedEvent.Invoke(*this, args);
}

void GameFoundation::EnqueueCreateSplashText(std::string text, const CIwFVec2& position) {
	m_xSplashtextCreationQueue.push(SplashTextInfo());
	SplashTextInfo& info = m_xSplashtextCreationQueue.back();
	info.text = text;
	info.position = position;
}

CIwFVec2 GameFoundation::GetGravity() {
	b2Vec2 b2g = m_xWorld.GetWorld().GetGravity();
	return CIwFVec2(b2g.x, b2g.y);
}

void GameFoundation::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;
	UpdatePhysics(frame.GetSimulatedDurationMs());
	ManageSpriteLifeCicles(frame);
}

void GameFoundation::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;
	
	// rendering all registered sprites
	for (SpriteMap::iterator it = m_xSpriteMap.begin(); it != m_xSpriteMap.end(); ++it) {
		it->second->Render(renderer, frame);
	}
}

void GameFoundation::UpdatePhysics(uint16 timestep) {
	int32 velocityIterations = 25;
    int32 positionIterations = 25;
	float32 box2dtimestep = ((float32)timestep)/1000.0f;
	m_xWorld.GetWorld().Step(box2dtimestep, velocityIterations, positionIterations);
    m_xWorld.GetWorld().ClearForces();
}

void GameFoundation::ManageSpriteLifeCicles(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	// find the bodies that can be destroyed, update the others
	SpriteMap::iterator it = m_xSpriteMap.begin();
	while (it != m_xSpriteMap.end()) {
		Sprite* sprite = it->second;

		// report a kill
		bool ishero = sprite->GetTypeName() == Hero::TypeName();
		bool outofbounds = CheckOutOfBounds(sprite->GetPosition());
		if (ishero && outofbounds) {
			Kill(dynamic_cast<Hero*>(sprite));
		}

		// delete the sprite, or update
		if (sprite->CanDispose() || outofbounds) {
			if (ishero) {
				m_iHeroCount--;
			}
			delete sprite;
			m_xSpriteMap.erase(it++);
		} else {
			sprite->Update(frame);
			++it;
		}
	}

	// create the queued splash texts
	while (m_xSplashtextCreationQueue.size()) {
		SplashTextInfo& info = m_xSplashtextCreationQueue.front();
		AddSplashText(info.text, info.position);
		m_xSplashtextCreationQueue.pop();
	}
}

bool GameFoundation::CheckOutOfBounds(const CIwFVec2& pos) {
	const float limit = 40.0f; // meters

	// only works if world is rooted at 0/0
	CIwFVec2 bounds(
		std::abs(m_xWorldSize.x) + (2.0f * limit), 
		std::abs(m_xWorldSize.y) + (2.0f * limit));
	
	return (std::abs(pos.x) + limit > bounds.x || std::abs(pos.y) + limit > bounds.y);
}

void GameFoundation::AddSplashText(std::string text, const CIwFVec2& position) {
	IW_CALLSTACK_SELF;

	SplashText* p = (SplashText*)FactoryManager::GetEffectFactory().Create("text");
	p->SetText(text);
	p->SetPosition(position);
	p->SetVelocity(CIwFVec2(0.0f, 0.8f));
	Add(p);
}

bool GameFoundation::RayHitTest(CIwFVec2 raystart, CIwFVec2 rayend) {
	return m_xRayCaster.RayHitTest(raystart, rayend);
}

void GameFoundation::Collide(Body& body1, Body& body2, bool issensorcollision, const CIwFVec2 collisionpoint, float approachvelocity) {
	IW_CALLSTACK_SELF;
	//IwTrace(MYAPP, ("Colliding: %s(%s) -> %s(%s)", body1.GetTypeName(), body1.GetId().c_str(), body2.GetTypeName(), body2.GetId().c_str()));

	// do body-internal actions (state transitions, texture changes, etc).
	body1.Collide(body2);

	// analyze hero impacts
	if (body1.GetTypeName() == Hero::TypeName()) {
		if (body2.GetTypeName() == Target::TypeName()) {
			Rescue(dynamic_cast<Hero*>(&body1));
		} else if (body2.GetTypeName() == Monster::TypeName()) {
			Kill(dynamic_cast<Hero*>(&body1));
//			args.soundgroup = "NinjaHurt";
		}
	}	

	// do sound event
	SoundEventArgs args;
	args.position = collisionpoint;
	args.impulse = std::min<float>(body1.GetMass(), body2.GetMass()) * approachvelocity;
	if (args.impulse > 0.001f) {
		SoundEffectEvent.Invoke(*this, args);
	}
}

void GameFoundation::CollisionEventHandler(const ContactListener& sender, const ContactListener::CollisionEventArgs& args) {
	Collide(*args.bodya, *args.bodyb, args.issensorcollision, args.collisionpoint, args.approachvelocity);
	Collide(*args.bodyb, *args.bodya, args.issensorcollision, args.collisionpoint, args.approachvelocity);
}
