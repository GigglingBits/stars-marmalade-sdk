#include "Touchpad.h"

Touchpad::Touchpad(const CIwSVec2& screensize) {
    // 20% of the screen is used for the touchpad
    m_xSize.x = screensize.x / 5;
    m_xSize.y = screensize.y;
}

bool Touchpad::HitTest(const CIwSVec2& screenpos) {
    CIwSVec2 diff = m_xSize - screenpos;
    return diff.x >= 0 && diff.x < m_xSize.x && diff.y >= 0 && diff.y < m_xSize.y;
}

void Touchpad::OnUpdate(const FrameData& frame) {

}

void Touchpad::OnRender(Renderer& renderer, const FrameData& frame) {
    VertexStreamScreen verts;
    verts.SetRect(0, 0, m_xSize.x, m_xSize.y);
    renderer.DrawPolygon(verts.GetVerts(), verts.GetVertCount(), 0xffffffff, 0x44aaaaaa);
}
