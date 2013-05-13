#include "Touchpad.h"

Touchpad::Touchpad() {
	m_xCenterPos = CIwSVec2::g_Zero;
	m_xSize = CIwSVec2::g_Zero;

	m_bIsTouching = false;
	m_xTouchPos = CIwSVec2::g_Zero;
	
	SetRederingLayer(Renderer::eRenderingLayerHud);
}

void Touchpad::SetPosition(const CIwSVec2& center) {
	m_xCenterPos = center;
}

void Touchpad::SetSize(const CIwSVec2& size) {
	m_xSize = size;
}

void Touchpad::SetTouch(const CIwSVec2& touchpos) {
	m_bIsTouching = true;
	m_xTouchPos = touchpos;
}

void Touchpad::UnsetTouch() {
	m_bIsTouching = false;
	m_xTouchPos = m_xCenterPos;
}

CIwFVec2 Touchpad::GetTouchVectorNormalized() {
	CIwSVec2 touchcenteroffset = m_xTouchPos - m_xCenterPos;
	
	CIwFVec2 touchvec;
	touchvec.x = touchcenteroffset.x / (m_xSize.x / 2.0f);
	touchvec.y = touchcenteroffset.y / (m_xSize.y / 2.0f);
	
	touchvec.x = std::max<float>(touchvec.x, -1.0f);
	touchvec.y = std::max<float>(touchvec.y, -1.0f);
	touchvec.x = std::min<float>(touchvec.x, 1.0f);
	touchvec.y = std::min<float>(touchvec.y, 1.0f);
	
	return touchvec;
}

bool Touchpad::HitTest(const CIwSVec2& touchpos) {
    CIwSVec2 diff = m_xCenterPos + CIwSVec2(m_xSize.x / 2, m_xSize.y / 2) - touchpos;
    return diff.x >= 0 && diff.x <= m_xSize.x && diff.y >= 0 && diff.y <= m_xSize.y;
}

void Touchpad::OnUpdate(const FrameData& frame) {
	if(!m_bIsTouching) {
		return;
	}

	// move the touchpad to optomal position (100% on screen)
	CIwSVec2 optimalpos;
	m_xCenterPos.x = std::max<int16>(m_xSize.x / 2, m_xCenterPos.x);
	m_xCenterPos.y = std::max<int16>(m_xSize.y / 2, m_xCenterPos.y);
	m_xCenterPos.x = std::min<int16>(frame.GetScreensize().x - (m_xSize.x / 2), m_xCenterPos.x);
	m_xCenterPos.y = std::min<int16>(frame.GetScreensize().y - (m_xSize.y / 2), m_xCenterPos.y);
}

void Touchpad::OnRender(Renderer& renderer, const FrameData& frame) {
	if(!m_bIsTouching) {
		return;
	}
	
    VertexStreamScreen verts;
    verts.SetRect(
				  m_xCenterPos.x - (m_xSize.x / 2),
				  m_xCenterPos.y - (m_xSize.y / 2),
				  m_xSize.x,
				  m_xSize.y);
    renderer.DrawPolygon(verts.GetVerts(), verts.GetVertCount(), 0xffffffff, 0x44aaaaaa);
}
