#include "Star.h"
#include "Nugget.h"
#include "StarState.h"
#include "FactoryManager.h"
#include "Configuration.h"

/****
 * Star main implementation
 **/
Star::Star(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef) 
	: CompositeBody(id, bodydef, fixturedef, texturedef), m_pxMotionState(NULL), m_pxParticles(NULL), m_bAutoOrient(false), m_uiShieldDuration(0), m_uiMagnetDuration(0) {

	GetBody().SetBullet(true); // improves collision detection

	SetGravityScale(0.0f);
		
	SetState(new InitialState(*this));
	GetHealthManager().SetResilience(0.0f);
	
	m_fAnchorLine = 0.0f;
	m_fPathSpeed = (float)Configuration::GetInstance().PathSpeed;
}

Star::~Star() {
	if (m_pxParticles) {
		delete m_pxParticles;
	}
	
	if (m_pxMotionState) {
		delete m_pxMotionState;
	}
}

const char* Star::GetTypeName() {
	return Star::TypeName();
}

const char* Star::TypeName() {
	static const char* type = "star";
	return type;
}

void Star::Initialize() {
	EndShield();
	EndMagnet();
}

void Star::AutoOrientTexture(bool allow) {
	m_bAutoOrient = allow;
}

void Star::Passify() {
	GetMotionState().Passify();
}

void Star::OnColliding(Body& body) {
	IW_CALLSTACK_SELF;

	GetMotionState().Collide(body);
	CompositeBody::OnColliding(body);
}

void Star::OnChildColliding(Body& child, Body& body) {
	if (dynamic_cast<Nugget*>(&body)) {
		if (0 == child.GetId().compare("magnet")) {
			Magnet.Invoke(*this, body.GetId());
		}
	}
}


void Star::OnUpdate(const FrameData& frame) {
	CompositeBody::OnUpdate(frame);
	GetMotionState().Update(frame.GetSimulatedDurationMs());
	
	// particle system
	if (m_pxParticles) {
		m_pxParticles->Update(frame);
	}
	
	// path
	m_xPath.Update(frame);
	
	// flip the texture according to movement
	if (!m_bAutoOrient) {
		// look right
		if (Texture* t = GetTexture()) {
			t->SetHorizontalFlip(false);
		}
	} else if (IsDragging()) {
		// look in drag direction
		CIwFVec2 target = GetDragTarget();
		if (Texture* t = GetTexture()) {
			t->SetHorizontalFlip(GetPosition().x - 0.2f > target.x);
		}
	} else {
		// look in movement direction
		if (Texture* t = GetTexture()) {
			t->SetHorizontalFlip(GetBody().GetLinearVelocity().x <= 0.0f);
		}
	}
	
	// shield count down
	if (m_uiShieldDuration != 0) {
		uint32 timestep = frame.GetAvgSimulatedDurationMs();
		if (m_uiShieldDuration > timestep) {
			m_uiShieldDuration -= timestep;
		} else {
			EndShield();
		}
	}

	// magnet count down
	if (m_uiMagnetDuration != 0) {
		uint32 timestep = frame.GetAvgSimulatedDurationMs();
		if (m_uiMagnetDuration > timestep) {
			m_uiMagnetDuration -= timestep;
		} else {
			EndMagnet();
		}
	}
}

void Star::OnRender(Renderer& renderer, const FrameData& frame) {
	if (m_pxParticles) {
		m_pxParticles->Render(renderer, frame);
	}
	m_xPath.Render(renderer, frame);
	CompositeBody::OnRender(renderer, frame);
}

void Star::SetAnchorLine(float xpos) {
	m_fAnchorLine = xpos;
	
	if (!m_xPath.IsWalking()) {
		CIwFVec2 anchortarget(GetDragTarget());
		anchortarget.x = m_fAnchorLine;
		MoveDragging(anchortarget);
	}
}

void Star::FollowPath(const std::vector<CIwFVec2>& path) {
	IW_CALLSTACK_SELF;
	if (path.size() < 1) {
		IwAssert(MYAPP, path.size() > 0);
		return;
	}
	
	// add lead-in
	std::vector<CIwFVec2> newpath;
	CIwFVec2 leadin = path[0] - GetPosition();
	CIwFVec2 step = leadin.GetNormalised() * PATHTRACKER_STEP_LENGTH;
	int stepcount = (int)(leadin.GetLength() / PATHTRACKER_STEP_LENGTH);
	leadin = step * stepcount; // // ensures that lead-in does not make path points jump
	newpath.push_back(path[0] - leadin);
		
	// copy new path
	newpath.insert(newpath.end(), path.begin(), path.end());
	m_xPath.ImportPath(newpath, leadin.GetLength());
	
	// get the star to pollow
	GetMotionState().FollowPath();
}

bool Star::IsFollowingPath() {
	// RRR: this should have been solved by checking the state, rather than
	///     the type! But I didn't see a good way to test the state.
	return m_xPath.IsWalking();
}

void Star::BeginShield(uint32 duration) {
	m_uiShieldDuration = duration;
	if (Body* shield = GetChild("shield")) {
		shield->SetTextureFrame("initiate");
        shield->EnableCollisions(true);
	}
}

void Star::EndShield() {
	m_uiShieldDuration = 0;
	if (Body* shield = GetChild("shield")) {
		shield->SetTextureFrame("burst");
        shield->EnableCollisions(false);
	}
}

void Star::BeginMagnet(uint32 duration) {
	m_uiMagnetDuration = duration;
	if (Body* field = GetChild("magnet")) {
		field->SetTextureFrame("on");
        field->EnableCollisions(true);
	}
}

void Star::EndMagnet() {
	m_uiMagnetDuration = 0;
	if (Body* field = GetChild("magnet")) {
		field->SetTextureFrame("off");
        field->EnableCollisions(false);
	}
}

void Star::EnableParticles() {
	if (!m_pxParticles) {
		m_pxParticles = new ParticleSystem(FactoryManager::GetTextureFactory().GetConfig("particle_yellow_star"), CIwFVec2(-20.0f, -5.0f), "Pop", "");
		m_pxParticles->SetBirthRate(30);
		m_pxParticles->SetParticleSize(CIwFVec2(0.2f, 0.2f));
		m_pxParticles->SetParticleSpeed(CIwFVec2(5.0f, 5.0f));
	}
	if (m_pxParticles) {
		m_pxParticles->Start();
	}
}

void Star::DisableParticles() {
	if (m_pxParticles) {
		m_pxParticles->Stop();
	}
}


/****
 * State machine 
 **/
void Star::SetState(Star::StateBase* newstate) {
	IwAssertMsg(MYAPP, newstate, ("Empty state must not be set."));
	if (m_pxMotionState) {
		delete m_pxMotionState;
	}
	newstate->Initialize();
	m_pxMotionState = newstate;
}

Star::StateBase& Star::GetMotionState() {
	IwAssertMsg(MYAPP, m_pxMotionState, ("Program error. State must not be empty."));
	return *m_pxMotionState;
}
