#ifndef __LEVELHUDNUMBER_H__
#define __LEVELHUDNUMBER_H__

#include "LevelHudText.h"

#define NUMBER_ROLL_TIME

class LevelHudNumber : public LevelHudText {
private:
	long m_lInternalNumber;
	long m_lDisplayedNumber;

	long m_lTotalRollTime;
	long m_lRemainingRollTime;
	
public:
	LevelHudNumber();
	
	void SetNumber(long number);
	void SetRollingNumber(long number, int milliseconds);

protected:
	virtual void OnUpdate(const FrameData& frame);
	
private:
	void UpdateText();
};

#endif
