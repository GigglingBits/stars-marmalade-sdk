#include "Sprite.h"
#include "Debug.h"
#include "IwDebug.h"

Sprite::Sprite(const std::string& id, const TextureTemplate& texturedef) : 
	m_eOrientation(eOrientationUndefined), m_xTexture(texturedef), m_sId(id) {
	m_xPosition= CIwFVec2::g_Zero;
	SetRederingLayer(Renderer::eRenderingLayerGameDynamicObjects);
}

Sprite::~Sprite() {
	// don't delete this destructor;
	// it is required to invoke destructors of derived classes when 
	// deleting a Sprite*
}

void Sprite::SetId(const std::string& id) {
	m_sId = id;
}

const std::string& Sprite::GetId() {
	return m_sId;
}

const char* Sprite::GetTypeName() {
	return Sprite::TypeName();
}

const char* Sprite::TypeName() {
	static const char* type = "sprite";
	return type;
}

bool Sprite::CanDispose() {
	return false;
}

Sprite::Orientation Sprite::GetOrientation() {
	return m_eOrientation;
}

void Sprite::SetPosition(const CIwFVec2& position) {
	m_xShape.MoveVerts(position - m_xPosition);
	m_xPosition = position;
}

CIwFVec2 Sprite::GetPosition() {
	return m_xPosition;
}

void Sprite::SetShape(CIwFVec2 verts[], int count, bool import) {
	m_xShape.SetVerts(verts, count, import);
}

void Sprite::SetShape(const VertexStreamWorld& shape) {
	m_xShape = shape;
}

void Sprite::SetOrientation(Orientation orientation) {
	if (m_eOrientation != orientation && orientation != eOrientationUndefined) {
		m_xTexture.SetHorizontalFlip(orientation == eOrientationLeft);
	}
	m_eOrientation = orientation;
}

Texture& Sprite::GetTexture() {
	return m_xTexture;
}

VertexStreamWorld& Sprite::GetShape() {
	return m_xShape;
}

void Sprite::OnUpdate(const FrameData& frame) {
	m_xTexture.Update(frame.GetSimulatedDurationMs());
}

void Sprite::OnRender(Renderer& renderer, const FrameData& frame) {
	IW_CALLSTACK_SELF;

	renderer.Draw(m_xShape, m_xTexture);

	// for debug:
	//renderer.DrawPolygon(
	//	m_xShape.GetVerts(), 
	//	m_xShape.GetVertCount(), 
	//	0xaa00ff00, 
	//	0x00000000);
	//renderer.DebugDrawCoords(*m_xShape.GetVerts());
}