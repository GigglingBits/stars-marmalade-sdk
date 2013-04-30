#include "AimingDevice.h"
#include "Debug.h"
#include "FactoryManager.h"
#include "Configuration.h"

AimingDevice::AimingDevice(GameFoundation& game) : m_xTrajectory(game) {
	m_pxTexture = FactoryManager::GetTextureFactory().Create("aimingdevice");
	IwAssertMsg(MYAPP, m_pxTexture, ("Texture for aiming device could not be created."));

	SetRederingLayer(Renderer::eRenderingLayerStaticGameObjects);

	m_xTextureOsc.Set(1.0f, 0.9f, 1.1f);
}

AimingDevice::~AimingDevice() {
	if (m_pxTexture) {
		delete m_pxTexture;
	}
}

void AimingDevice::Show() {
	if (!m_bShown) {
		m_xTextureOsc.Restart();
	}
	m_bShown = true;
}

void AimingDevice::Hide() {
	m_bShown = false;
}

void AimingDevice::SetPosition(const CIwFVec2& worldpos) {
	m_xPosition = worldpos;
	m_xTrajectory.SetPosition(worldpos);
}

void AimingDevice::SetVelocity(const CIwFVec2& impulse, float mass){
	m_xTrajectory.SetVelocity(impulse, mass);
}

void AimingDevice::OnUpdate(const FrameData& frame) {
	IW_CALLSTACK_SELF;

	m_xTrajectory.Update(frame);
	UpdateCrosshair(frame);
}

void AimingDevice::UpdateCrosshair(const FrameData& frame) {
	m_xTextureOsc.Update(frame.GetSimulatedDurationMs());
	if (m_bShown) {
		float range = m_xTextureOsc.GetValue();
		CIwFVec2 verts[4];
		verts[0].x = m_xPosition.x - range;
		verts[0].y = m_xPosition.y;
		verts[1].x = m_xPosition.x;
		verts[1].y = m_xPosition.y - range;
		verts[2].x = m_xPosition.x + range;
		verts[2].y = m_xPosition.y;
		verts[3].x = m_xPosition.x;
		verts[3].y = m_xPosition.y + range;
		m_xShape.SetVerts(verts, 4);
	}
	m_pxTexture->Update(frame.GetSimulatedDurationMs());
}

void AimingDevice::OnRender(Renderer& renderer, const FrameData& frame) {
	if (!m_bShown) {
		return;
	}

	IW_CALLSTACK_SELF;

	if (m_pxTexture) {
		renderer.Draw(m_xShape, *m_pxTexture);
	}

	m_xTrajectory.Render(renderer, frame);
}		
