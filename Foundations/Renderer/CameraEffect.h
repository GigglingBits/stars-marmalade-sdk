#ifndef __CAMERAEFFECT_H__
#define __CAMERAEFFECT_H__

#include "IwGeomFVec2.h"
#include "Oscillator.h"

class CameraEffect {
private:
	int m_iDuration;
	int m_iTimeRemaining;

	float m_fAmplitude;
	
	Oscillator m_xXOscillator;
	Oscillator m_xYOscillator;
	
	CIwFVec2 m_xOffeset;
	
public:
	CameraEffect();
	
	void StartQuake(float amplitude, uint16 duration);
	
	bool IsActive();
	
	const CIwFVec2& GetEffectOffset();
	
	void Update(const CIwSVec2& screensize, uint16 timestep);
};

#endif
