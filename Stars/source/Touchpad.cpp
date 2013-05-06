#include "Touchpad.h"

Touchpad::Touchpad() {
	m_xCenterPos = CIwSVec2::g_Zero;
	m_xSize = CIwSVec2::g_Zero;
	m_xTouchPos = CIwSVec2::g_Zero;
	SetRederingLayer(Renderer::eRenderingLayerHud);
}

void Touchpad::SetPosition(const CIwSVec2& center) {
	m_xCenterPos = center;
}

void Touchpad::SetSize(const CIwSVec2& size) {
	m_xSize = size;
}

bool Touchpad::SetTouch(const CIwSVec2& screenpos) {
	m_xTouchPos = screenpos;
	return HitTest(screenpos);
}

CIwFVec2 Touchpad::GetTouchVectorNormalized() {
	CIwSVec2 touchcenteroffset = m_xTouchPos - m_xCenterPos;
	
	CIwFVec2 touchvec;
	touchvec.x = touchcenteroffset.x / (m_xSize.x / 2);
	touchvec.y = touchcenteroffset.y / (m_xSize.y / 2);
	return touchvec;
}

bool Touchpad::HitTest(const CIwSVec2& screenpos) {
    CIwSVec2 diff = m_xCenterPos + CIwSVec2(m_xSize.x / 2, m_xSize.y / 2) - screenpos;
    return diff.x >= 0 && diff.x <= m_xSize.x && diff.y >= 0 && diff.y <= m_xSize.y;
}

void Touchpad::OnUpdate(const FrameData& frame) {
}

void Touchpad::OnRender(Renderer& renderer, const FrameData& frame) {
    VertexStreamScreen verts;
    verts.SetRect(
				  m_xCenterPos.x - (m_xSize.x / 2),
				  m_xCenterPos.y - (m_xSize.y / 2),
				  m_xSize.x,
				  m_xSize.y);
    renderer.DrawPolygon(verts.GetVerts(), verts.GetVertCount(), 0xffffffff, 0x44aaaaaa);
}
