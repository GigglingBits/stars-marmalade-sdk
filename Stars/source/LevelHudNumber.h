#ifndef __LEVELHUDNUMBER_H__
#define __LEVELHUDNUMBER_H__

#include "LevelHudText.h"

#define NUMBER_ROLL_TIME 1500 // ms for rolling the number

class LevelHudNumber : public LevelHudText {
private:
	long m_lInternalNumber;
	long m_lDisplayedNumber;

	long m_lRemainingRollTime;
	
public:
	LevelHudNumber();
	
	void SetNumber(long number);
	void SetRollingNumber(long number);

protected:
	virtual void OnUpdate(const FrameData& frame);
	
private:
	void UpdateText();
};

#endif
