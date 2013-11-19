#ifndef __LEVELHUDNUMBER_H__
#define __LEVELHUDNUMBER_H__

#include "LevelHudText.h"

#define NUMBER_ROLL_TIME

class LevelHudNumber : public LevelHudText {
private:
	long m_lTargetNumber;
	long m_lLastTargetNumber;

	long m_lTotalRollTime;
	long m_lRemainingRollTime;
	
public:
	LevelHudNumber();
	
	void SetNumber(long number, int rolltime = 0);

protected:
	virtual void OnUpdate(const FrameData& frame);
	
private:
	void UpdateText();
};

#endif
