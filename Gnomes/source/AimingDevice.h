#ifndef __AIMINGDEVICE_H__
#define __AIMINGDEVICE_H__

#include "Renderable.h"
#include "Texture.h"
#include "VertexStreamWorld.h"
#include "Oscillator.h"
#include "Trajectory.h"

class AimingDevice : public Renderable {
private:
	bool m_bShown;
	CIwFVec2 m_xPosition;

	Texture* m_pxTexture;
	VertexStreamWorld m_xShape;
	Oscillator m_xTextureOsc;

	Trajectory m_xTrajectory;

public:
	AimingDevice(GameFoundation& game);
	~AimingDevice();

	void Show();
	void Hide();

	void SetPosition(const CIwFVec2& worldpos);
	void SetVelocity(const CIwFVec2& impulse, float mass);

private:
	void UpdateCrosshair(const FrameData& frame);

protected:
	virtual void OnUpdate(const FrameData& frame);
	virtual void OnRender(Renderer& renderer, const FrameData& frame);
};

#endif
