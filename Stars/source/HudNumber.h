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
	
	std::string m_sPrefix;
	std::string m_sPostfix;
	
public:
	HudNumber();
	
	void SetNumber(long number, int rolltime = 0);
	long GetNumber();

	void SetPrefix(const std::string text);
	void SetPostfix(const std::string text);
	
protected:
	virtual void OnUpdate(const FrameData& frame);
	
private:
	void UpdateText();
};

#endif
