#include "LocalEffect.h"
#include "Debug.h"

LocalEffect::LocalEffect(const std::string& id, const ShapeTemplate& shapedef, const TextureTemplate& texturedef)
	: Sprite(id, texturedef), m_bCanDispose(false) {

	m_iRemaingTimeMS = 0x7fff;

	const int count = 4;
	CIwFVec2 verts[count];
	shapedef.CreateAABB(verts);
	GetShape().SetVerts(verts, count, false);

	SetRederingLayer(Renderer::eRenderingLayerForegroundObjects);
	
	m_xVelocity = CIwFVec2::g_Zero;
}

const char* LocalEffect::GetTypeName() {
	return LocalEffect::TypeName();
}

const char* LocalEffect::TypeName() {
	static const char* type = "localeffect";
	return type;
}

void LocalEffect::SetVelocity(const CIwFVec2& v) {
	m_xVelocity = v;
}

void LocalEffect::SetFadeTime(uint16 ms) {
	m_iRemaingTimeMS = ms;
}

bool LocalEffect::CanDispose() {
	return m_bCanDispose;
}

void LocalEffect::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;
	Sprite::OnUpdate(frame);

	// fading
	uint16 timestep = frame.GetSimulatedDurationMs();
	if (m_iRemaingTimeMS < timestep) {
		m_iRemaingTimeMS = 0;
		m_bCanDispose = true;
	} else {
		m_iRemaingTimeMS -= timestep;
	}

	// velocity
	if (!m_xVelocity.IsZero()) {
		CIwFVec2 pos = GetPosition();
		pos.x += (m_xVelocity.x * (float)timestep) / 1000.0f;
		pos.y += (m_xVelocity.y * (float)timestep) / 1000.0f;
		SetPosition(pos);
	}
}
