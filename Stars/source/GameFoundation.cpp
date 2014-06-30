#include <sstream>

#include "Main.h"
#include "GameFoundation.h"
#include "SplashText.h"
#include "FactoryManager.h"
#include "Configuration.h"
#include "Nugget.h"
#include "BuffMagnet.h"
#include "BuffShield.h"
#include "BuffShoot.h"

#include "Debug.h"
#include "IwDebug.h"

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

void GameFoundation::RegisterStar(Star* star) {
	m_pxStar = star;
	m_pxStar->Killed.AddListener(this, &GameFoundation::StarKilledEventHandler);
	m_pxStar->DustEvent.AddListener(this, &GameFoundation::DustEventHandler);
}

void GameFoundation::UnregisterStar() {
	m_pxStar->DustEvent.RemoveListener(this, &GameFoundation::DustEventHandler);
	m_pxStar->Killed.RemoveListener(this, &GameFoundation::StarKilledEventHandler);
	m_pxStar = NULL;
}

void GameFoundation::Add(Body* body) {
	// indexing of star
	if (body->GetTypeName() == Star::TypeName()) {
		IwAssertMsg(MYAPP, !m_pxStar, ("There seems to be a star already. There should by not more than one star at any given time."));
		RegisterStar((Star*)body);
	} else if (Buff* buff = dynamic_cast<Buff*>(body)) {
		buff->Collected.AddListener(this, &GameFoundation::BuffCollectedEventHandler);
	}

	body->EmitBuffRequested.AddListener(this, &GameFoundation::BuffRequestedEventHandler);
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
				UnregisterStar();
			}
			if (Body* body = dynamic_cast<Body*>(sprite)) {
				body->EmitBuffRequested.RemoveListener(this, &GameFoundation::BuffRequestedEventHandler);
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

int GameFoundation::GetDustMultiplier(int queuedcount) {
	// the longer the queue, the more points to get
	if (queuedcount < 4) { return 1; }
	if (queuedcount < 8) { return 2; }
	return 5;
}

void GameFoundation::EnqueueDust(const CIwFVec2& pos, int amount) {
	int multiplier = GetDustMultiplier(m_xDust.GetQueuedDustCount() + 1);
	int multipliedamount = amount * multiplier;
	m_xDust.EnqueueDust(multipliedamount);
	
	// display
	CreatePointSplash(amount, multiplier, pos);
}

void GameFoundation::CommitDust(const CIwFVec2& pos) {
	m_xDust.CommitDustQueue();
}

void GameFoundation::CancelDust(const CIwFVec2& pos) {
	if (m_xDust.GetQueuedDustCount() <= 0) {
		return;
	}

	// cancel
	CreatePointSplash(-m_xDust.GetQueuedDustAmount(), 1, pos);
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

void GameFoundation::CreatePointSplash(int amount, int bonusmultiplier, const CIwFVec2& position) {
	IW_CALLSTACK_SELF;
	IwAssert(MYAPP, amount != 0);
	IwAssert(MYAPP, bonusmultiplier >= 0);
	IwAssert(MYAPP, (amount > 0 && bonusmultiplier > 0) || amount < 0);
	
	if (amount == 0 || bonusmultiplier == 0) {
		return;
	}

	std::ostringstream oss;
	if (amount > 0) {
		oss << amount;
		CreateSplashText(oss.str(), position, GAME_COLOUR_FONT_MAIN, Renderer::eFontTypeNormal);
		if (bonusmultiplier > 1) {
			oss.str(std::string());
			oss << "x" << bonusmultiplier;
			CreateSplashText(oss.str(), position + CIwFVec2(0.3f, -0.1f), GAME_COLOUR_FONT_BONUS, Renderer::eFontTypeSmall);
		}
	} else {
		oss << amount;
		CreateSplashText(oss.str(), position, GAME_COLOUR_FONT_PENALTY, Renderer::eFontTypeSmall);
	}
}

void GameFoundation::CreateSplashText(std::string text, const CIwFVec2& position, uint32 colour, Renderer::FontType font) {
	IW_CALLSTACK_SELF;
	SplashText* p = (SplashText*)FactoryManager::GetEffectFactory().Create("text");
	if (p) {
		p->SetText(text, colour, font);
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
	// evaluate probability
	float probability = 1.0f;
	float random = (float)(rand() % 100) / 100.0f;
	if (probability <= random) {
		return;
	}
	
	// select buff (based on even/odd number )
	std::string buff;
	switch ((int)(random * 100.0f)%3) {
	case 0:
		buff = "buff_magnet";
		break;
	case 1:
		buff = "buff_shield";
		break;
	case 2:
		buff = "buff_shoot";
		break;
	}
		
	// create buff
	CIwFVec2 speed(0.0f, Configuration::GetInstance().BuffSpeed);
	EnqueueCreateBody(buff, pos, speed);
}

void GameFoundation::ActivateMagnetBuff() {
	if (Star* star = GetStar()) {
		CreateSplashText("Magnet", star->GetPosition(), GAME_COLOUR_FONT_MAIN, Renderer::eFontTypeSmall);
	}
}

void GameFoundation::ActivateShieldBuff() {
	if (Star* star = GetStar()) {
		CreateSplashText("Shield", star->GetPosition(), GAME_COLOUR_FONT_MAIN, Renderer::eFontTypeSmall);
	}
}

void GameFoundation::ActivateShootBuff() {
	if (Star* star = GetStar()) {
		CreateSplashText("Shoot", star->GetPosition(), GAME_COLOUR_FONT_MAIN, Renderer::eFontTypeSmall);
	}
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

void GameFoundation::StarKilledEventHandler(const Star& sender, const int& args) {
	UnregisterStar();
}

void GameFoundation::BuffRequestedEventHandler(const Body& sender, const Body::EmitBuffArgs& args) {
	EmitBuff(args.pos);
}

void GameFoundation::BuffCollectedEventHandler(const Buff& sender, const Buff::BuffArgs& args) {
	IW_CALLSTACK_SELF;
	
	// figure out which buff was collected
	BuffType bt;
	if (dynamic_cast<BuffMagnet*>((Buff*)&sender)) {
		bt = eBuffTypeMagnet;
	} else if (dynamic_cast<BuffShield*>((Buff*)&sender)) {
		bt = eBuffTypeShield;
	} else if (dynamic_cast<BuffShoot*>((Buff*)&sender)) {
		bt = eBuffTypeShoot;
	} else {
		IwAssertMsg(MYAPP, false, ("Unkown buff type found. The buff will be ignored."));
		return;
	}
	
	// notify the
	BuffCollected.Invoke(*this, bt);
}

void GameFoundation::EffectRequestedEventHandler(const Body& sender, const Body::EffectArgs& args) {
	Sprite* fx = FactoryManager::GetEffectFactory().Create(args.id);
	fx->SetPosition(args.pos);
	Add(fx);
}
