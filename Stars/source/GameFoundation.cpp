#include <sstream>

#include "Main.h"
#include "GameFoundation.h"
#include "SplashText.h"
#include "FactoryManager.h"
#include "Configuration.h"
#include "Nugget.h"
#include "Enemy.h"
#include "BuffMagnet.h"
#include "BuffShield.h"
#include "BuffShoot.h"

#include "Debug.h"
#include "IwDebug.h"

GameFoundation::GameFoundation(float dustrequirement, const CIwFVec2& worldsize) :
m_xContactHandler(m_xWorld),
m_xWorldSize(worldsize),
m_xRayCaster(m_xWorld), m_pxStar(NULL),
m_xDust(dustrequirement),
m_uiShootBuffTimer(0) {
}

GameFoundation::~GameFoundation() {
	SpriteMap::iterator it;
	for (it = m_xSpriteMap.begin(); it != m_xSpriteMap.end(); ++it) {
		Sprite* sprite = it->second;
		if (Body* body = dynamic_cast<Body*>(sprite)) {
			body->DustEvent.RemoveListener(this, &GameFoundation::DustEventHandler);
		}
		delete sprite;
	}
}

void GameFoundation::Initialize() {
	m_xWorld.SetContactListener(m_xContactHandler);

	m_xShootBuffCurtain.SetRenderingLayer(Renderer::eRenderingLayerBackground);
	m_xShootBuffCurtain.SetFadeTime(200);
	m_xShootBuffCurtain.SetAlpha(0x99);
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
	m_pxStar->Magnet.AddListener(this, &GameFoundation::StarMagnetEventHandler);
}

void GameFoundation::UnregisterStar() {
	m_pxStar->Magnet.RemoveListener(this, &GameFoundation::StarMagnetEventHandler);
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
	body->DustEvent.AddListener(this, &GameFoundation::DustEventHandler);

	Add((Sprite*)body);
}

void GameFoundation::Add(Sprite* sprite) {
	// make sure it has an id
	std::string id = sprite->GetId();
	IwAssertMsg(MYAPP, !id.empty(), ("A sprite could not be added to the game because no id was defined."));

	// register the body
	m_xSpriteMap[id] = sprite;
	
	// notify
	SpriteAdded.Invoke(*this, *sprite);
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
	uint32 simulframetime = frame.GetSimulatedDurationMs();
	
	UpdateMagnet(simulframetime);
	UpdateShoot(simulframetime);

	UpdatePhysics(simulframetime);
	ManageSpriteLifeCicles(frame);

	m_xShootBuffCurtain.Update(frame);
}

void GameFoundation::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;
	m_xShootBuffCurtain.Render(renderer, frame);

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


void GameFoundation::UpdateMagnet(uint16 timestep) {
	IW_CALLSTACK_SELF;
	
	if (m_xMagnetSprites.empty()) {
		return;
	}
	
	std::list<std::string>::iterator mit = m_xMagnetSprites.begin();
	while (mit != m_xMagnetSprites.end()) {
		Nugget* nugget;
		SpriteMap::iterator sit = m_xSpriteMap.find(*mit);
		if (sit != m_xSpriteMap.end() && sit->second && (nugget = dynamic_cast<Nugget*>(sit->second))) {
			if (m_pxStar) {
				// star is alive -> regular update
				nugget->SetMagnetPosition(m_pxStar->GetPosition());
			} else {
				// star has died -> remove nugget
				nugget->CancelMagnet();
			}
			++mit;
		} else {
			m_xMagnetSprites.erase(mit++);
		}
	}
}

void GameFoundation::UpdateShoot(uint16 timestep) {
	IW_CALLSTACK_SELF;

	// shoot buff
	if (m_uiShootBuffTimer <= 0) {
		return;
	}

	if (m_uiShootBuffTimer > timestep) {
		m_uiShootBuffTimer -= timestep;
	} else {
		m_uiShootBuffTimer = 0;
		m_xShootBuffCurtain.Open();

		// do the killing
		std::list<std::string>::iterator shootit = m_xShootSprites.begin();
		while (shootit != m_xShootSprites.end()) {
			Enemy* enemy;
			SpriteMap::iterator sit = m_xSpriteMap.find(*shootit);
			if (sit != m_xSpriteMap.end() && sit->second && (enemy = dynamic_cast<Enemy*>(sit->second))) {
				enemy->Shoot();
			}
			m_xShootSprites.erase(shootit++);
		}
	}
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
				body->DustEvent.RemoveListener(this, &GameFoundation::DustEventHandler);
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

void GameFoundation::BeginDustQueue() {
	m_xDust.BeginDustQueue();
}

void GameFoundation::AddDust(const CIwFVec2& pos, int amount) {
	int multiplier = 1;
	if (m_xDust.IsQueueing()) {
		multiplier = GetDustMultiplier(m_xDust.GetQueuedDustCount() + 1);
	}

	int multipliedamount = amount * multiplier;
	m_xDust.AddDust(multipliedamount);
	
	CreatePointSplash(amount, multiplier, pos);
}

void GameFoundation::CommitDustQueue(const CIwFVec2& pos) {
	m_xDust.CommitDustQueue();
}

void GameFoundation::RollbackDustQueue(const CIwFVec2& pos) {
	if (!m_xDust.IsQueueing()) {
		return;
	}

	int queueddustamount = m_xDust.GetQueuedDustAmount();
	m_xDust.RollbackDustQueue();

	if (queueddustamount > 0) {
		// cancel
		CreatePointSplash(-queueddustamount, 1, pos);

		// quake effect
		QuakeImpactArgs arg;
		arg.amplitude = 0.3f;
		QuakeImpact.Invoke(*this, arg);
	}
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
			CreateSplashText(oss.str(), position + CIwFVec2(0.4f, -0.3f), GAME_COLOUR_FONT_BONUS, Renderer::eFontTypeSmall);
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

void GameFoundation::EmitBuff(const CIwFVec2& pos, const Body::BuffProbabilities& probs) {
	IW_CALLSTACK_SELF;
	// evaluate probability
	float totalprobability = probs.magnet + probs.shield + probs.shoot;
	if (totalprobability <= 0.0f) {
		return;
	}
	
	float scale = 1.0f;
	if (totalprobability > 1.0f) {
		scale = 1.0 / totalprobability;
	}

	float random = (float)(rand() % 100) / 100.0f;
	float probability = 0.0f;
	std::string buff;

	probability += probs.magnet * scale;
	if (probability >= random) {
		buff = "buff_magnet";
	} else {
		probability += probs.shield * scale;
		if (probability >= random) {
			buff = "buff_shield";
		} else {
			probability += probs.shoot * scale;
			if (probability >= random) {
				buff = "buff_shoot";
			} else {
				// its ok to not emit a buff, if the total probability is smaller than 1.0
				IwAssert(MYAPP, totalprobability < 1.0f);
				return;
			}
		}
	}
	
	// create buff
	CIwFVec2 speed(0.0f, Configuration::GetInstance().BuffSpeed);
	EnqueueCreateBody(buff, pos, speed);
}

void GameFoundation::ActivateMagnetBuff() {
	if (Star* star = GetStar()) {
		star->BeginMagnet(Configuration::GetInstance().BuffMagnetDuration);
	}
}

void GameFoundation::ActivateShieldBuff() {
	if (Star* star = GetStar()) {
		star->BeginShield(Configuration::GetInstance().BuffShieldDuration);
	}
}

void GameFoundation::ActivateShootBuff() {
	m_uiShootBuffTimer	= Configuration::GetInstance().BuffShootDuration;
	m_xShootBuffCurtain.Close();

	if (!m_pxStar) {
		return;
	}
	
	// find all enemies and their distances to star
	EnemyRefs enemies;
	for (SpriteMap::iterator it = m_xSpriteMap.begin(); it != m_xSpriteMap.end(); ++it) {
		if (it->second) {
			if (dynamic_cast<Enemy*>(it->second)) {
				EnemyRef enemyref;
				enemyref.BodyId = it->first;
				enemyref.Distance = (it->second->GetPosition() - m_pxStar->GetPosition()).GetLength();
				enemies.push_back(enemyref);
			}
		}
	}
		
	// remove farest enemies
	while (enemies.size() > Configuration::GetInstance().BuffShootCount) {
		EnemyRefs::iterator eraseit = enemies.end();
		float distance = 0.0f;
		for (EnemyRefs::iterator it = enemies.begin(); it != enemies.end(); it++) {
			if (it->Distance > distance) {
				distance = it->Distance;
				eraseit = it;
			}
		}
		if (eraseit != enemies.end()) {
			enemies.erase(eraseit);
		}
	}

	// add remaining enemies to shoot list
	for (EnemyRefs::iterator it = enemies.begin(); it != enemies.end(); it++) {
		m_xShootSprites.push_back(it->BodyId);
	}
}

void GameFoundation::DustEventHandler(const Body& sender, const Star::DustEventArgs& args) {
	switch (args.eventtype) {
		case Body::eDustEventTypeAdd: {
			AddDust(args.position, args.amount);
			break;
		}

		case Body::eDustEventTypeBegin: {
			BeginDustQueue();
			break;
		}

		case Body::eDustEventTypeCommit: {
			CommitDustQueue(args.position);
			break;
		}

		case Body::eDustEventTypeRollback: {
			RollbackDustQueue(args.position);
			break;
		}
	}
}

void GameFoundation::StarKilledEventHandler(const Star& sender, const int& args) {
	UnregisterStar();
}

void GameFoundation::StarMagnetEventHandler(const Star& sender, const std::string& args) {
	// args is name id of the sprite that entered the magetic field
	m_xMagnetSprites.push_back(args);
}

void GameFoundation::BuffRequestedEventHandler(const Body& sender, const Body::EmitBuffArgs& args) {
	EmitBuff(args.pos, args.probs);
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
