
#include "GameFoundation.h"
#include "SplashText.h"
#include "FactoryManager.h"
#include "SoundEngine.h"

GameFoundation::GameFoundation(const CIwFVec2& worldsize) 
	: m_xWorldSize(worldsize), m_xRayCaster(m_xWorld), m_pxStar(NULL) {

	m_xContactListener.CollisionEvent.AddListener<GameFoundation>(this, &GameFoundation::CollisionEventHandler);
	m_xWorld.SetContactListener(&m_xContactListener);

	m_xBodyTimer.Elapsed.AddListener(this, &GameFoundation::BodyTimerEventHandler);
	m_xBodyTimer.LastEventFired.AddListener(this, &GameFoundation::BodyTimerClearedEventHandler);
}

GameFoundation::~GameFoundation() {
	m_xBodyTimer.LastEventFired.RemoveListener(this, &GameFoundation::BodyTimerClearedEventHandler);
	m_xBodyTimer.Elapsed.RemoveListener(this, &GameFoundation::BodyTimerEventHandler);

	SpriteMap::iterator it;
	for (it = m_xSpriteMap.begin(); it != m_xSpriteMap.end(); ++it) {
		Sprite* sprite = it->second;
		delete sprite;
	}

	m_xWorld.SetContactListener(NULL);
	m_xContactListener.CollisionEvent.RemoveListener<GameFoundation>(this, &GameFoundation::CollisionEventHandler);
}

bool GameFoundation::IsCompleted() {
	return !m_pxStar || (m_xBodyTimer.GetTotalDuration() < m_xBodyTimer.GetElapsedTime());
}

float GameFoundation::GetCompletionDegree() {
	uint32 total = m_xBodyTimer.GetTotalDuration();
	return total == 0 ? 1.0f : std::min<float>(1.0f, (float)m_xBodyTimer.GetElapsedTime() / (float)total);
}

const GameFoundation::CompletionInfo& GameFoundation::GetCompletionInfo() {
	return m_xCompletionInfo;
}

std::map<std::string, Sprite*>& GameFoundation::GetSpriteMap() {
	return m_xSpriteMap;
}

Star* GameFoundation::GetStar() {
	return m_pxStar;
}

Sprite* GameFoundation::FindSprite(const std::string& id) {
	SpriteMap::iterator it = m_xSpriteMap.find(id);
	if (it == m_xSpriteMap.end()) {
		return NULL;
	}
	return it->second;
}

void GameFoundation::Add(Body* body) {
	// indexing of star
	if (body->GetTypeName() == Star::TypeName()) {
		IwAssertMsg(MYAPP, !m_pxStar, ("There seems to be a star already. There should by not more than one star at any given time."));
		m_pxStar = (Star*)body;
	}

	body->SetGameFoundation(*this);

	Add((Sprite*)body);
}

void GameFoundation::Add(Sprite* sprite) {
	// make sure it has an id
	std::string id = sprite->GetId();
	IwAssertMsg(MYAPP, !id.empty(), ("A sprite could not be added to the game because no id was defined."));

	// register the body
	m_xSpriteMap[id] = sprite;
}

void GameFoundation::Add(uint16 delay, const std::string& body, float ypos) {
	IW_CALLSTACK_SELF;

	BodySpec spec;
	spec.Body = body;
	spec.YPos = ypos;

	m_xBodyTimer.Enqueue(delay, spec);
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
	m_xBodyTimer.Update(frame.GetSimulatedDurationMs());
}

void GameFoundation::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;
	
	// rendering all registered sprites
	for (SpriteMap::iterator it = m_xSpriteMap.begin(); it != m_xSpriteMap.end(); ++it) {
		it->second->Render(renderer, frame);
	}
}

void GameFoundation::UpdatePhysics(uint16 timestep) {
	const int32 velocityIterations = 25;
    const int32 positionIterations = 25;
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

		bool outofbounds = CheckOutOfBounds(sprite->GetPosition());
		if (outofbounds || sprite->CanDispose()) {
			if (m_pxStar == sprite) {
				m_pxStar = NULL;
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
	const float limit = 30.0f; // meters

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
	IwTrace(MYAPP, ("Colliding: %s(%s) -> %s(%s)", body1.GetTypeName(), body1.GetId().c_str(), body2.GetTypeName(), body2.GetId().c_str()));

	// do body-internal actions (state transitions, texture changes, etc).
	body1.Collide(body2);

	// general collision actions
	SoundEngine::PlaySoundEffect("collision");
}

void GameFoundation::CollisionEventHandler(const ContactListener& sender, const ContactListener::CollisionEventArgs& args) {
	Collide(*args.bodya, *args.bodyb, args.issensorcollision, args.collisionpoint, args.approachvelocity);
	Collide(*args.bodyb, *args.bodya, args.issensorcollision, args.collisionpoint, args.approachvelocity);
}

void GameFoundation::BodyTimerEventHandler(const EventTimer<BodySpec>& sender, const BodySpec& args) {
	IW_CALLSTACK_SELF;

	BodyFactory& factory = FactoryManager::GetBodyFactory();		
	if (Body* body = factory.Create(args.Body)) {
		body->SetPosition(CIwFVec2(m_xWorldSize.x * 2, args.YPos));
		body->SetSpeed(CIwFVec2(-4.0f, 0.0f));
		Add(body);
	} else {
		IwAssertMsg(MYAPP, body, ("Failed to create new body with name '%s'", args.Body.c_str()));
	}
}

void GameFoundation::BodyTimerClearedEventHandler(const EventTimer<BodySpec>& sender, const int& dummy) {
	m_xCompletionInfo.IsCleared = true;
}
