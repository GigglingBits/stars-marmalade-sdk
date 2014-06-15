#ifndef __HUDNUMBER_H__
#define __HUDNUMBER_H__

#include "HudText.h"

#define NUMBER_ROLL_TIME

class HudNumber : public HudText {
private:
	long m_lTargetNumber;
	long m_lLastTargetNumber;

	long m_lTotalRollTime;
	long m_lRemainingRollTime;
	
public:
	HudNumber();
	
	void SetNumber(long number, int rolltime = 0);

protected:
	virtual void OnUpdate(const FrameData& frame);
	
private:
	void UpdateText();
};

#endif
