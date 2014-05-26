#include <sstream>

#include "GameFoundation.h"
#include "SplashText.h"
#include "FactoryManager.h"
#include "Configuration.h"
#include "Nugget.h"

GameFoundation::GameFoundation(float dustrequirement, const CIwFVec2& worldsize) :
m_xContactHandler(m_xWorld),
m_xWorldSize(worldsize),
m_xRayCaster(m_xWorld), m_pxStar(NULL),
m_xDust(dustrequirement) {
}

GameFoundation::~GameFoundation() {
	if (m_pxStar) {
		m_pxStar->DustEvent.RemoveListener(this, &GameFoundation::DustEventHandler);
	}
	
	SpriteMap::iterator it;
	for (it = m_xSpriteMap.begin(); it != m_xSpriteMap.end(); ++it) {
		Sprite* sprite = it->second;
		delete sprite;
	}
}

void GameFoundation::Initialize() {
	m_xWorld.SetContactListener(m_xContactHandler);
}

std::map<std::string, Sprite*>& GameFoundation::GetSpriteMap() {
	return m_xSpriteMap;
}

Star* GameFoundation::GetStar() {
	return m_pxStar;
}

bool GameFoundation::StarHitTest(CIwFVec2 position) {
	Star* star = GetStar();
	return star && star->HitTest(position);
}

Sprite* GameFoundation::FindSprite(const std::string& id) {
	SpriteMap::iterator it = m_xSpriteMap.find(id);
	if (it == m_xSpriteMap.end()) {
		return NULL;
	}
	return it->second;
}

bool GameFoundation::IsGameGoing() {
	// the game is going if there is at least one
	// dynamic, non-star body within the world bounds
	for (SpriteMap::iterator it = m_xSpriteMap.begin(); it != m_xSpriteMap.end(); it++) {
		if (Body* body = dynamic_cast<Body*>(it->second)) {
			if (!CheckOutOfWorld(body->GetPosition())) {
				b2Body& b2body = body->GetBody();
				if (b2body.IsActive() && b2body.GetType() == b2_dynamicBody) {
					if (!dynamic_cast<Star*>(body)) {
						IwTrace(MYAPP, ("Game is going because of body '%s' of type '%s'", body->GetId().c_str(), body->GetTypeName()));
						return true;
					}
				}
			}
		}
	}
	return false;
}

void GameFoundation::Add(Body* body) {
	// indexing of star
	if (body->GetTypeName() == Star::TypeName()) {
		IwAssertMsg(MYAPP, !m_pxStar, ("There seems to be a star already. There should by not more than one star at any given time."));
		m_pxStar = (Star*)body;
		m_pxStar->DustEvent.AddListener(this, &GameFoundation::DustEventHandler);
	}

	body->BuffRequested.AddListener(this, &GameFoundation::BuffRequestedEventHandler);
	body->EffectRequested.AddListener(this, &GameFoundation::EffectRequestedEventHandler);

	Add((Sprite*)body);
}

void GameFoundation::Add(Sprite* sprite) {
	// make sure it has an id
	std::string id = sprite->GetId();
	IwAssertMsg(MYAPP, !id.empty(), ("A sprite could not be added to the game because no id was defined."));

	// register the body
	m_xSpriteMap[id] = sprite;
}

void GameFoundation::EnqueueCreateSplashText(std::string text, const CIwFVec2& position) {
	m_xSplashtextCreationQueue.push(SplashTextInfo());
	SplashTextInfo& info = m_xSplashtextCreationQueue.back();
	info.text = text;
	info.position = position;
}

void GameFoundation::EnqueueCreateBody(std::string id, const CIwFVec2& position, const CIwFVec2& speed) {
	m_xBodyCreationQueue.push(BodyInfo());
	BodyInfo& info = m_xBodyCreationQueue.back();
	info.id = id;
	info.position = position;
	info.speed = speed;
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
	const int32 velocityIterations = 10;
    const int32 positionIterations = 10;
	float32 box2dtimestep = ((float32)timestep) / 1000.0f;
	m_xWorld.GetWorld().Step(box2dtimestep, velocityIterations, positionIterations);
    m_xWorld.GetWorld().ClearForces();
}

void GameFoundation::ManageSpriteLifeCicles(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	// find the bodies that can be destroyed, update the others
	SpriteMap::iterator it = m_xSpriteMap.begin();
	while (it != m_xSpriteMap.end()) {
		Sprite* sprite = it->second;

		bool outofbounds = CheckOutOfUniverse(sprite->GetPosition());
		if (outofbounds || sprite->CanDispose()) {
			if (m_pxStar == sprite) {
				m_pxStar->DustEvent.RemoveListener(this, &GameFoundation::DustEventHandler);
				m_pxStar = NULL;
			}
			if (Body* body = dynamic_cast<Body*>(sprite)) {
				body->BuffRequested.RemoveListener(this, &GameFoundation::BuffRequestedEventHandler);
				body->EffectRequested.RemoveListener(this, &GameFoundation::EffectRequestedEventHandler);
			}
			
			SpriteRemovedArgs args;
			args.sprite = sprite;
			args.outofbounds = outofbounds;
			SpriteRemoved.Invoke(*this, args);
			
			delete sprite;
			m_xSpriteMap.erase(it++);
		} else {
			sprite->Update(frame);
			++it;
		}
	}
	
	// create the queued bodies
	while (m_xBodyCreationQueue.size()) {
		BodyInfo& info = m_xBodyCreationQueue.front();
		CreateBody(info.id, info.position, info.speed);
		m_xBodyCreationQueue.pop();
	}
	
	// create the queued splash texts
	while (m_xSplashtextCreationQueue.size()) {
		SplashTextInfo& info = m_xSplashtextCreationQueue.front();
		CreateSplashText(info.text, info.position);
		m_xSplashtextCreationQueue.pop();
	}
}

float GameFoundation::GetDustFillPercent() {
	return m_xDust.GetDustFillPercent();
}

float GameFoundation::GetDustFillAmount() {
	return m_xDust.GetCollectedDustAmount();
}

float GameFoundation::GetDustFillMax() {
	return m_xDust.GetMaxDustAmount();
}

float GameFoundation::GetDustQueuedAmount() {
	return m_xDust.GetQueuedDustAmount();
}

float GameFoundation::GetDustQueuedPercent() {
	return m_xDust.GetQueuedDustPercent();
}

void GameFoundation::EnqueueDust(const CIwFVec2& pos, int amount) {
	int multipliedamount = (m_xDust.GetQueuedDustCount() + 1) * amount;
	m_xDust.EnqueueDust(multipliedamount);
	CreateSplashNumber(multipliedamount, pos, 0xffccfaff);
}

void GameFoundation::CommitDust(const CIwFVec2& pos) {
	m_xDust.CommitDustQueue();
}

void GameFoundation::CancelDust(const CIwFVec2& pos) {
	if (m_xDust.GetQueuedDustCount() <= 0) {
		return;
	}

	// cancel
	CreateSplashNumber(-m_xDust.GetQueuedDustAmount(), pos, 0xff0000ff);
	m_xDust.ClearDustQueue();

	// quake effect
	QuakeImpactArgs arg;
	arg.amplitude = 0.3f;
	QuakeImpact.Invoke(*this, arg);
}

bool GameFoundation::CheckOutOfWorld(const CIwFVec2& pos) {
	// The world is defined as the area where interaction with
	// a body can be expected.
	return CheckOutOfBounds(pos, 2.0f);
}

bool GameFoundation::CheckOutOfUniverse(const CIwFVec2& pos) {
	// The universe is the world area plus a generous margin.
	// Bodies outside the universe are not expected to play any
	// role in the game.
	return CheckOutOfBounds(pos, 10.0f);
}

bool GameFoundation::CheckOutOfBounds(const CIwFVec2& pos, float margin) {
	// only works if world is rooted at 0/0
	CIwFVec2 bounds(
		std::abs(m_xWorldSize.x) + (2.0f * margin),
		std::abs(m_xWorldSize.y) + (2.0f * margin));
	
	return (std::abs(pos.x) + margin > bounds.x || std::abs(pos.y) + margin > bounds.y);
}

void GameFoundation::CreateSplashNumber(long number, const CIwFVec2& position, uint32 colour) {
	IW_CALLSTACK_SELF;
		
	std::ostringstream oss;
	oss << number;
	CreateSplashText(oss.str(), position, colour);
}

void GameFoundation::CreateSplashText(std::string text, const CIwFVec2& position, uint32 colour) {
	IW_CALLSTACK_SELF;
	
	SplashText* p = (SplashText*)FactoryManager::GetEffectFactory().Create("text");
	if (p) {
		p->SetText(text, colour);
		p->SetPosition(position);
		p->SetVelocity(CIwFVec2(0.0f, 0.8f));
		Add(p);
	}
}

void GameFoundation::CreateBody(std::string id, const CIwFVec2& position, const CIwFVec2& speed) {
	IW_CALLSTACK_SELF;
	
	Body* p = (Body*)FactoryManager::GetBodyFactory().Create(id);
	if (p) {
		p->SetPosition(position);
		p->GetBody().SetLinearVelocity(b2Vec2(speed.x, speed.y));
		Add(p);
	}
}

bool GameFoundation::RayHitTest(CIwFVec2 raystart, CIwFVec2 rayend) {
	return m_xRayCaster.RayHitTest(raystart, rayend);
}

void GameFoundation::EmitBuff(const CIwFVec2& pos) {
	IW_CALLSTACK_SELF;
/*
	// evaluate probability
	float probability = m_xDust.GetDustFillPercent();
	float random = (float)(rand() % 100) / 100.0f;
	if (probability <= random) {
		return;
	}
	
	// select buff (based on even/odd number )
	std::string buff = ((int)(random * 100.0f))%2 ? "buff_attack" : "buff_block";
	
	// create buff
	CIwFVec2 speed(0.0f, Configuration::GetInstance().BuffSpeed);
	EnqueueCreateBody(buff, pos, speed);
*/
}

void GameFoundation::DustEventHandler(const Star& sender, const Star::DustEventArgs& args) {
	switch (args.EventType) {
		case Star::eDustEventTypeCollectSingle: {
			IwAssert(MYAPP, m_xDust.GetQueuedDustCount() == 0);
			EnqueueDust(args.position, args.amount);
			CommitDust(args.position);
			break;
		}
			
		case Star::eDustEventTypeCollect: {
			EnqueueDust(args.position, args.amount);
			break;
		}
			
		case Star::eDustEventTypeCommit: {
			CommitDust(args.position);
			break;
		}
				
		case Star::eDustEventTypeRollback: {
			CancelDust(args.position);
			break;
		}
	}
}

void GameFoundation::BuffRequestedEventHandler(const Body& sender, const Body::BuffArgs& args) {
	EmitBuff(args.pos);
}

void GameFoundation::EffectRequestedEventHandler(const Body& sender, const Body::EffectArgs& args) {
	Sprite* fx = FactoryManager::GetEffectFactory().Create(args.id);
	fx->SetPosition(args.pos);
	Add(fx);
}
