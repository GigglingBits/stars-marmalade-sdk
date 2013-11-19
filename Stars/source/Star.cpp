#include "Star.h"
#include "Target.h"
#include "Buff.h"
#include "StarMotionState.h"
#include "StarAttackState.h"
#include "FactoryManager.h"

/****
 * Star main implementation
 **/
Star::Star(const std::string& id, const b2BodyDef& bodydef, const b2FixtureDef& fixturedef, const TextureTemplate& texturedef) 
	: Body(id, bodydef, fixturedef, texturedef), m_pxMotionState(NULL), m_pxAttackState(NULL), m_pxParticles(NULL) {
	GetBody().SetBullet(true);
	SetState(new RetractingState(*this));
	SetState(new PeacefulState(*this));
	GetHealthManager().SetResilience(0.0f);
	m_pxTouchTexture = FactoryManager::GetTextureFactory().Create("touch");
		
	m_fAnchorLine = 0.0f;
}

Star::~Star() {
	if (m_pxTouchTexture) {
		delete m_pxTouchTexture;
		m_pxTouchTexture = NULL;
	}

	if (m_pxParticles) {
		delete m_pxParticles;
	}
	
	if (m_pxMotionState) {
		delete m_pxMotionState;
	}

	if (m_pxAttackState) {
		delete m_pxAttackState;
	}
}

const char* Star::GetTypeName() {
	return Star::TypeName();
}

const char* Star::TypeName() {
	static const char* type = "star";
	return type;
}

void Star::OnColliding(Body& thisbody, Body& otherbody) {
	IW_CALLSTACK_SELF;

	GetAttackState().Collide(otherbody);

	if (otherbody.GetTypeName() == Buff::TypeName()) {
		GetHealthManager().RenewLife();
		otherbody.GetHealthManager().Kill();
		if (m_pxMotionState) {
			m_pxMotionState->IncrementMultiplier();
		}
	} else if (otherbody.GetTypeName() == Target::TypeName()) {
		GetHealthManager().RenewLife();
	} else {
		GetHealthManager().Injure(5.0f);
	}
}

void Star::OnUpdate(const FrameData& frame) {
	Body::OnUpdate(frame);
	GetMotionState().Update(frame.GetSimulatedDurationMs());
	GetAttackState().Update(frame.GetSimulatedDurationMs());

	// adjust drag force
	if (IsDragging()) {
		float distance = (GetDragTarget() - GetPosition()).GetLength();
		SetDragForce(
			distance				// the larger the distance, the larger the force to be applied
			* 100.0f				// just a constant to amplify the effect
			* GetMass());			// larger bodies require more force to move
	}
	
	// particle system
	if (m_pxParticles) {
		m_pxParticles->Update(frame);
	}
	
	// flip the texture according to movement
	if (IsDragging()) {
		// look in drag direction
		CIwFVec2 target = GetDragTarget();
		GetTexture().SetHorizontalFlip(GetPosition().x - 0.2f > target.x);
	} else {
		// look in movement direction
		GetTexture().SetHorizontalFlip(GetBody().GetLinearVelocity().x <= 0.0f);
	}
}

void Star::SetMotionTextureFrame(const std::string& name) {
	m_sMotionTextureFrame = name;
	if (m_sAttackTextureFrame.empty()) {
		SetTextureFrame(m_sMotionTextureFrame);
	}
}

void Star::SetAttackTextureFrame(const std::string& name) {
	m_sAttackTextureFrame = name;
	SetTextureFrame(m_sAttackTextureFrame);
}

void Star::ClearAttackTextureFrame() {
	m_sAttackTextureFrame.clear();
	SetTextureFrame(m_sMotionTextureFrame);
}

void Star::SetTextureFrame(std::string id) {
	GetTexture().SelectFrame(id, GetHealthManager().GetHealthValue());
}

void Star::OnRender(Renderer& renderer, const FrameData& frame) {
	if (IsDragging()) {
		CIwFVec2* center = GetPort("center");
		CIwFVec2 target = GetDragTarget();

		renderer.DrawLine(
			GetPosition() + (center ? *center : CIwFVec2::g_Zero),
			target,
			0xffaaaaaa);

		if (m_pxTouchTexture) {
			VertexStreamWorld verts;
			verts.SetRect(target.x - 0.2f, target.y - 0.2f, 0.4f, 0.4f);
			renderer.Draw(verts, *m_pxTouchTexture);
		}
	}

	Body::OnRender(renderer, frame);

	if (m_pxParticles) {
		m_pxParticles->Render(renderer, frame);
	}
}

void Star::SetAnchorLine(float xpos) {
	m_fAnchorLine = xpos;
	
	if (m_xPath.empty()) {
		CIwFVec2 anchortarget(GetDragTarget());
		anchortarget.x = m_fAnchorLine;
		MoveDragging(anchortarget);
	}
}

void Star::FollowPath(int samplecount, const CIwFVec2* samplepoints, float speed) {
	// clear current path
	while (!m_xPath.empty()) {
		m_xPath.pop();
	}
	
	// copy new path
	m_fPathSpeed = speed;
	for (int i = 0; i < samplecount; i++) {
		m_xPath.push(samplepoints[i]);
	}
	
	// get the star to pollow
	GetMotionState().FollowPath();
}

bool Star::IsFollowingPath() {
	// RRR: this should have been solved by checking the state, rather than
	///     the type! But I didn't find a simple way to test the state.
	return !m_xPath.empty();
}

void Star::ShowTextEffect(const std::string& text) {
	IW_CALLSTACK_SELF;
	if (GameFoundation* game = GetGameFoundation()) {
		game->AddSplashText(text, GetPosition());
	}	
}

void Star::BeginBlock() {
	GetAttackState().BeginBlock();
}

void Star::EndBlock() {
	GetAttackState().EndBlock();
}

void Star::BeginAttack() {
	GetAttackState().BeginAttack();
}

void Star::EndAttack() {
	GetAttackState().EndAttack();
}

void Star::EnableParticles() {
	if (!m_pxParticles) {
		m_pxParticles = new ParticleSystem(FactoryManager::GetTextureFactory().GetConfig("spark"), CIwFVec2(-20.0f, -5.0f));
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
void Star::SetState(Star::MotionStateBase* newstate) {
	IwAssertMsg(MYAPP, newstate, ("Empty state must not be set."));
	if (m_pxMotionState) {
		delete m_pxMotionState;
	}
	newstate->Initialize();
	m_pxMotionState = newstate;
}

void Star::SetState(Star::AttackStateBase* newstate) {
	IwAssertMsg(MYAPP, newstate, ("Empty state must not be set."));
	if (m_pxAttackState) {
		delete m_pxAttackState;
	}
	newstate->Initialize();
	m_pxAttackState = newstate;
}

Star::MotionStateBase& Star::GetMotionState() {
	IwAssertMsg(MYAPP, m_pxMotionState, ("Program error. State must not be empty."));
	return *m_pxMotionState;
}

Star::AttackStateBase& Star::GetAttackState() {
	IwAssertMsg(MYAPP, m_pxAttackState, ("Program error. State must not be empty."));
	return *m_pxAttackState;
}
