#ifndef __OSCILLATOR_H__ 
#define __OSCILLATOR_H__ 

#include "IwTypes.h"

#define OSC_BASE_RESOLUTION 500

class Oscillator {
private:
	static float s_afSinus[OSC_BASE_RESOLUTION];
	static bool s_bInitialized;

	float m_fFrequency;
	int m_iCursor;

	float m_fMinVal;
	float m_fMaxVal;

	float m_fCurrentValue;

	void InitCache();
	void UpdateCurentValue();

public:
	Oscillator(float frequency = 1.0f, float minval = 0.0f, float maxval = 1.0f);

	void Restart();
	void Set(float frequency, float minval = 0.0f, float maxval = 1.0f);

	float GetValue();
	void Update(uint16 timestep);
};

#endif
